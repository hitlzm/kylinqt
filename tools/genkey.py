#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""登录密钥摘要生成工具（配合 auth/AuthManager 使用）

作用：把操作员实际使用的登录密钥，离线换算成一串 32 字节十六进制摘要。
      只有摘要会被写进 C++ 源码和 git；登录密钥本身不落地、不外传。

用法（项目根目录下，请在终端里直接运行）：
    python tools/genkey.py               # 交互输入，输入时不回显（推荐）
    python tools/genkey.py --visible     # 终端无法隐藏输入时用，会明文回显
    python tools/genkey.py --stdin       # 从管道读入，仅用于自动化测试
    python tools/genkey.py --check       # 核对某串口令能否通过 AuthManager 的校验
    python tools/genkey.py --self-test   # 自检：用 RFC 4231 标准向量验证本脚本

换密钥时重新跑一遍，把打印出来的 kKeyDigestHex 那一行替换到
auth/AuthManager.cpp 的同名常量处，盐（SALT）保持不变 —— 盐变了摘要就全变了，
必须和 C++ 侧完全一致。
"""

import argparse
import getpass
import hashlib
import hmac
import os
import re
import sys

# 项目根目录（本脚本位于 <root>/tools/）
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AUTH_CPP = os.path.join(PROJECT_ROOT, "auth", "AuthManager.cpp")

# ── 盐 ────────────────────────────────────────────────────────────────
# 用途：让摘要只对本项目有效，避免被网上的彩虹表/预计算表反查。
# 加密学上不需要保密（它本来就要写进程序二进制里），但是：
#   * 换登录密钥时不要改动它；
#   * AuthManager.cpp 里必须是同一串，两侧一致才能校验成功。
SALT = "8x102FOBMtL8NuYZ"

# 建议的最短长度：低于这个值枚举空间太小，摘要形同虚设（见 README 说明）
MIN_LEN = 12

# 枚举速度假设：每秒 10 亿次。远超普通 PC 单核，用来做保守估计。
ATTACK_RATE = 1e9


def digest_of(password: str, salt: str = SALT) -> str:
    """HMAC-SHA256(盐, 密钥) 的小写十六进制表示，与 Qt 的
    QMessageAuthenticationCode::hash(...).toHex() 输出一致。"""
    return hmac.new(
        salt.encode("utf-8"), password.encode("utf-8"), hashlib.sha256
    ).hexdigest()


def strength(password: str):
    """粗略估计枚举代价：按字符集大小 ^ 长度算组合数。"""
    charset = 0
    if any(c.isdigit() for c in password):
        charset += 10
    if any(c.islower() for c in password):
        charset += 26
    if any(c.isupper() for c in password):
        charset += 26
    if any(not c.isalnum() for c in password):
        charset += 33
    # 排除明显的连号/重复，避免高估（如 aaaaaaaa 实际只有一个组合）
    distinct = len(set(password))
    effective = min(charset, max(distinct, 1) * 4)

    space = float(effective) ** len(password)
    seconds = space / ATTACK_RATE
    if seconds < 1:
        human = "%.2f 秒（几乎瞬间）" % seconds
    elif seconds < 3600:
        human = "%.1f 分钟" % (seconds / 60)
    elif seconds < 86400 * 365:
        human = "%.1f 天" % (seconds / 86400)
    else:
        human = "%.2e 年" % (seconds / 31536000.0)
    return effective, space, human


def report(password: str) -> bool:
    """打印强度评估，返回是否达到建议标准。"""
    effective, space, human = strength(password)
    print("  长度      : %d" % len(password))
    print("  有效字符集: 约 %d 种" % effective)
    print("  组合数    : %.2e" % space)
    print("  枚举耗时  : 以每秒 10 亿次计算，约 %s" % human)

    ok = True
    if len(password) < MIN_LEN:
        print("  [!] 长度不足 %d 位。摘要的安全性完全依赖枚举代价，" % MIN_LEN)
        print("      短密钥可以在几秒内被还原（6 位数字实测 2.66 秒）。")
        ok = False
    if effective <= 10:
        print("  [!] 字符种类太少（例如纯数字），枚举空间被大幅压缩。")
        ok = False
    if password.lower() in (
        "123456",
        "password",
        "admin",
        "kylin",
        "kylinqt",
    ):
        print("  [!] 这是常见口令，必须更换。")
        ok = False
    return ok


def read_secret(prompt: str, use_stdin: bool, visible: bool) -> str:
    """读取密钥输入，尽量不回显。

    正常路径用 getpass（Windows 下直接读控制台，绕过管道）；
    某些终端（如 MSYS2 mintty）拿不到控制台时 getpass 会失败，
    此时用 --visible 走明文 input()，否则会一直等在那里不动。
    """
    if use_stdin:
        line = sys.stdin.readline()
        if not line:
            raise EOFError("stdin 已结束")
        return line.rstrip("\r\n")
    if visible:
        return input(prompt)
    try:
        return getpass.getpass(prompt)
    except Exception as exc:  # 终端不支持隐藏输入
        print("\n[提示] 当前终端无法隐藏输入（%s）。" % exc)
        print("       如确认周围无人可见，请用 --visible 重新运行；")
        print("       否则请换到系统自带的 cmd / PowerShell 窗口再试。")
        raise


def do_check(auth_file: str, use_stdin: bool, visible: bool) -> int:
    """读取 AuthManager.cpp 里的盐与摘要，核对输入口令是否能通过登录。"""
    try:
        with open(auth_file, encoding="utf-8") as fh:
            src = fh.read()
    except OSError as exc:
        print("[错误] 读不到 %s：%s" % (auth_file, exc))
        return 1

    m_salt = re.search(r'kKeySalt\s*=\s*"([^"]*)"', src)
    m_digest = re.search(r'kKeyDigestHex\s*=\s*"([0-9a-fA-F]*)"', src)
    if not m_salt or not m_digest:
        print("[错误] 在 %s 里找不到 kKeySalt / kKeyDigestHex" % auth_file)
        return 1

    salt = m_salt.group(1)
    stored = m_digest.group(1).lower()

    print("=" * 66)
    print("登录口令核对")
    print("=" * 66)
    print("程序里的盐    : %s" % salt)
    print("程序里的摘要  : %s（%d 字符）" % (stored, len(stored)))
    if len(stored) != 64:
        print("[!] 摘要长度不是 64，可能填错了")
    print()

    try:
        password = read_secret("请输入要核对的口令: ", use_stdin, visible)
    except (KeyboardInterrupt, EOFError) as exc:
        print("\n已取消（%s）。" % exc)
        return 1

    got = digest_of(password, salt)
    print("\n该口令的摘要  : %s" % got)
    if got == stored:
        print("\n匹配 —— 这串口令可以登录。")
        return 0

    print("\n不匹配 —— 这串口令登录不了。")
    print("若确认口令没输错，说明 AuthManager.cpp 里的摘要与它不成对：")
    print("  运行 python tools/genkey.py 重新生成摘要，替换 kKeyDigestHex 那一行。")
    return 2


def do_self_test() -> int:
    """用 RFC 4231 Test Case 2 的标准向量验证实现是否规范。"""
    expected = "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843"
    got = digest_of("what do ya want for nothing?", salt="Jefe")
    print("RFC 4231 测试向量")
    print("  期望: %s" % expected)
    print("  实际: %s" % got)
    if got == expected:
        print("  结果: 通过 —— 本脚本与 Qt 的 QMessageAuthenticationCode 同为标准 HMAC-SHA256")
        return 0
    print("  结果: 失败")
    return 1


def main() -> int:
    parser = argparse.ArgumentParser(description="生成 AuthManager 使用的密钥摘要")
    parser.add_argument(
        "--self-test", action="store_true", help="用标准向量验证本脚本"
    )
    parser.add_argument(
        "--force", action="store_true", help="即使强度不足也输出摘要"
    )
    parser.add_argument(
        "--visible", action="store_true", help="明文回显输入（终端不支持隐藏输入时）"
    )
    parser.add_argument(
        "--stdin", action="store_true", help="从标准输入读取密钥，仅用于自动化测试"
    )
    parser.add_argument(
        "--check", action="store_true", help="核对口令能否通过 AuthManager 的校验"
    )
    parser.add_argument(
        "--auth-file", default=AUTH_CPP,
        help="核对模式读取的文件，默认 auth/AuthManager.cpp"
    )
    args = parser.parse_args()

    if args.self_test:
        return do_self_test()

    if args.check:
        return do_check(args.auth_file, args.stdin, args.visible)

    print("=" * 66)
    print("登录密钥摘要生成")
    print("=" * 66)
    print("提示：输入的密钥不会显示、不会保存、不会写入任何文件；")
    print("      生成后请自行抄写在纸上或存进密码管理器。")
    print()

    try:
        pw1 = read_secret("请输入登录密钥: ", args.stdin, args.visible)
        pw2 = read_secret("请再输入一遍确认: ", args.stdin, args.visible)
    except (KeyboardInterrupt, EOFError) as exc:
        print("\n已取消（%s）。" % exc)
        return 1

    if pw1 != pw2:
        print("\n[错误] 两次输入不一致。密钥无法找回，请重新运行。")
        return 1

    if not pw1:
        print("\n[错误] 密钥为空。")
        return 1

    print("\n强度评估：")
    ok = report(pw1)

    if not ok and not args.force:
        print()
        print("建议换一个更长、更随机的密钥（例如 16 位大小写字母+数字）。")
        print("确实要用这个密钥，请加 --force 重新运行。")
        return 2

    print()
    print("=" * 66)
    print("把下面这行替换到 auth/AuthManager.cpp 的同名常量处：")
    print("=" * 66)
    print()
    print('const char *const kKeySalt      = "%s";' % SALT)
    print('const char *const kKeyDigestHex = "%s";' % digest_of(pw1))
    print()
    print("盐（kKeySalt）请勿改动，改了摘要就失效。")
    print("可运行 python tools/genkey.py --check 核对新口令。")
    return 0


if __name__ == "__main__":
    sys.exit(main())
