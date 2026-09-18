# -*- coding: utf-8 -*-
"""把 out/*.csv 画成一张 PNG（仅用标准库：手写 PNG 编码 + 5x7 点阵字）。"""
import os
import zlib
import struct
import math

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'out')

# ───────────────────────── 5x7 点阵字 ─────────────────────────
FONT = {
    'A': ['.###.', '#...#', '#...#', '#####', '#...#', '#...#', '#...#'],
    'B': ['####.', '#...#', '#...#', '####.', '#...#', '#...#', '####.'],
    'C': ['.####', '#....', '#....', '#....', '#....', '#....', '.####'],
    'D': ['####.', '#...#', '#...#', '#...#', '#...#', '#...#', '####.'],
    'E': ['#####', '#....', '#....', '####.', '#....', '#....', '#####'],
    'F': ['#####', '#....', '#....', '####.', '#....', '#....', '#....'],
    'G': ['.###.', '#...#', '#....', '#.###', '#...#', '#...#', '.###.'],
    'H': ['#...#', '#...#', '#...#', '#####', '#...#', '#...#', '#...#'],
    'I': ['#####', '..#..', '..#..', '..#..', '..#..', '..#..', '#####'],
    'J': ['..###', '...#.', '...#.', '...#.', '...#.', '#..#.', '.##..'],
    'K': ['#...#', '#..#.', '#.#..', '##...', '#.#..', '#..#.', '#...#'],
    'L': ['#....', '#....', '#....', '#....', '#....', '#....', '#####'],
    'M': ['#...#', '##.##', '#.#.#', '#...#', '#...#', '#...#', '#...#'],
    'N': ['#...#', '##..#', '#.#.#', '#..##', '#...#', '#...#', '#...#'],
    'O': ['.###.', '#...#', '#...#', '#...#', '#...#', '#...#', '.###.'],
    'P': ['####.', '#...#', '#...#', '####.', '#....', '#....', '#....'],
    'Q': ['.###.', '#...#', '#...#', '#...#', '#.#.#', '#..#.', '.##.#'],
    'R': ['####.', '#...#', '#...#', '####.', '#.#..', '#..#.', '#...#'],
    'S': ['.####', '#....', '#....', '.###.', '....#', '....#', '####.'],
    'T': ['#####', '..#..', '..#..', '..#..', '..#..', '..#..', '..#..'],
    'U': ['#...#', '#...#', '#...#', '#...#', '#...#', '#...#', '.###.'],
    'V': ['#...#', '#...#', '#...#', '#...#', '#...#', '.#.#.', '..#..'],
    'W': ['#...#', '#...#', '#...#', '#...#', '#.#.#', '##.##', '#...#'],
    'X': ['#...#', '#...#', '.#.#.', '..#..', '.#.#.', '#...#', '#...#'],
    'Y': ['#...#', '#...#', '.#.#.', '..#..', '..#..', '..#..', '..#..'],
    'Z': ['#####', '....#', '...#.', '..#..', '.#...', '#....', '#####'],
    '0': ['.###.', '#...#', '#..##', '#.#.#', '##..#', '#...#', '.###.'],
    '1': ['..#..', '.##..', '..#..', '..#..', '..#..', '..#..', '.###.'],
    '2': ['.###.', '#...#', '....#', '...#.', '..#..', '.#...', '#####'],
    '3': ['#####', '...#.', '..#..', '...#.', '....#', '#...#', '.###.'],
    '4': ['...#.', '..##.', '.#.#.', '#..#.', '#####', '...#.', '...#.'],
    '5': ['#####', '#....', '####.', '....#', '....#', '#...#', '.###.'],
    '6': ['..##.', '.#...', '#....', '####.', '#...#', '#...#', '.###.'],
    '7': ['#####', '....#', '...#.', '..#..', '.#...', '.#...', '.#...'],
    '8': ['.###.', '#...#', '#...#', '.###.', '#...#', '#...#', '.###.'],
    '9': ['.###.', '#...#', '#...#', '.####', '....#', '...#.', '.##..'],
    ' ': ['.....', '.....', '.....', '.....', '.....', '.....', '.....'],
    '.': ['.....', '.....', '.....', '.....', '.....', '.##..', '.##..'],
    '-': ['.....', '.....', '.....', '.###.', '.....', '.....', '.....'],
    '+': ['.....', '..#..', '..#..', '#####', '..#..', '..#..', '.....'],
    '=': ['.....', '.....', '.###.', '.....', '.###.', '.....', '.....'],
    ':': ['.....', '.##..', '.##..', '.....', '.##..', '.##..', '.....'],
    '(': ['..##.', '.#...', '.#...', '.#...', '.#...', '.#...', '..##.'],
    ')': ['.##..', '...#.', '...#.', '...#.', '...#.', '...#.', '.##..'],
    '/': ['....#', '....#', '...#.', '..#..', '.#...', '#....', '#....'],
    '<': ['....#', '...#.', '..#..', '.#...', '..#..', '...#.', '....#'],
    '>': ['#....', '.#...', '..#..', '...#.', '..#..', '.#...', '#....'],
    ',': ['.....', '.....', '.....', '.....', '.##..', '.##..', '.#...'],
}


class Canvas:
    def __init__(self, w, h, bg=(255, 255, 255)):
        self.w, self.h = w, h
        self.buf = bytearray(bg * (w * h))

    def px(self, x, y, c):
        x, y = int(x), int(y)
        if 0 <= x < self.w and 0 <= y < self.h:
            i = (y * self.w + x) * 3
            self.buf[i:i + 3] = bytes(c)

    def line(self, x0, y0, x1, y1, c, width=1):
        x0, y0, x1, y1 = int(round(x0)), int(round(y0)), int(round(x1)), int(round(y1))
        dx, dy = abs(x1 - x0), abs(y1 - y0)
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        err = dx - dy
        while True:
            if width == 1:
                self.px(x0, y0, c)
            else:
                for ox in range(width):
                    for oy in range(width):
                        self.px(x0 + ox, y0 + oy, c)
            if x0 == x1 and y0 == y1:
                break
            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x0 += sx
            if e2 < dx:
                err += dx
                y0 += sy

    def rect(self, x0, y0, x1, y1, c, fill=False):
        if fill:
            for y in range(int(y0), int(y1) + 1):
                for x in range(int(x0), int(x1) + 1):
                    self.px(x, y, c)
        else:
            self.line(x0, y0, x1, y0, c)
            self.line(x1, y0, x1, y1, c)
            self.line(x1, y1, x0, y1, c)
            self.line(x0, y1, x0, y0, c)

    def text(self, x, y, s, c=(0, 0, 0), scale=1):
        for ch in s.upper():
            g = FONT.get(ch, FONT[' '])
            for r, row in enumerate(g):
                for k, p in enumerate(row):
                    if p == '#':
                        for sx in range(scale):
                            for sy in range(scale):
                                self.px(x + k * scale + sx, y + r * scale + sy, c)
            x += 6 * scale

    def save(self, path):
        raw = bytearray()
        for y in range(self.h):
            raw.append(0)
            raw += self.buf[y * self.w * 3:(y + 1) * self.w * 3]

        def chunk(tag, data):
            return (struct.pack('>I', len(data)) + tag + data +
                    struct.pack('>I', zlib.crc32(tag + data) & 0xFFFFFFFF))

        png = b'\x89PNG\r\n\x1a\n'
        png += chunk(b'IHDR', struct.pack('>IIBBBBB', self.w, self.h, 8, 2, 0, 0, 0))
        png += chunk(b'IDAT', zlib.compress(bytes(raw), 9))
        png += chunk(b'IEND', b'')
        with open(path, 'wb') as f:
            f.write(png)


def read_csv(name):
    path = os.path.join(OUT, name)
    with open(path, 'r') as f:
        lines = [l.strip() for l in f if l.strip()]
    names = lines[0].split(',')
    cols = {n: [] for n in names}
    for l in lines[1:]:
        vals = [float(v) for v in l.split(',')]
        for n, v in zip(names, vals):
            cols[n].append(v)
    return cols


RED = (200, 30, 30)
BLUE = (30, 90, 200)
GREEN = (20, 140, 60)
GRAY = (150, 150, 150)
DARK = (40, 40, 40)
ORANGE = (230, 130, 0)


def draw_panel(c, x, y, w, h, title, xs, ys_list, colors, xr, yr,
               xlabel='', ylabel='', zero_line=True, subtitle=''):
    c.rect(x, y, x + w, y + h, (210, 210, 210))
    c.rect(x, y, x + w, y + h, (210, 210, 210))
    c.text(x, y - 24, title, DARK, 2)
    if subtitle:
        c.text(x + 6, y + 6, subtitle, GRAY, 1)

    def mx(v):
        return x + (v - xr[0]) / (xr[1] - xr[0]) * w

    def my(v):
        return y + h - (v - yr[0]) / (yr[1] - yr[0]) * h

    # 网格 + 刻度
    for i in range(5):
        v = xr[0] + (xr[1] - xr[0]) * i / 4.0
        gx = mx(v)
        c.line(gx, y, gx, y + h, (238, 238, 238))
        c.text(gx - 10, y + h + 4, ('%g' % round(v, 3)), GRAY, 1)
    for i in range(5):
        v = yr[0] + (yr[1] - yr[0]) * i / 4.0
        gy = my(v)
        c.line(x, gy, x + w, gy, (238, 238, 238))
        c.text(x - 44, gy - 3, ('%g' % round(v, 4)), GRAY, 1)

    if zero_line and yr[0] < 0 < yr[1]:
        c.line(x, my(0), x + w, my(0), (170, 170, 170))

    for xs_, ys_, col in zip(xs, ys_list, colors):
        px = py = None
        for xv, yv in zip(xs_, ys_):
            if xv is None or yv is None:
                px = py = None
                continue
            if xv < xr[0] or xv > xr[1]:
                continue
            cy = my(max(yr[0], min(yr[1], yv)))
            cx = mx(xv)
            if px is not None:
                c.line(px, py, cx, cy, col)
            px, py = cx, cy

    c.text(x + w - 62, y - 22, xlabel, GRAY, 1)
    if ylabel:
        c.text(x - 46, y - 22, ylabel, GRAY, 1)


def main():
    W, H = 1680, 1180
    c = Canvas(W, H)
    c.text(30, 24, 'FILTER PREDICTION TEST  -  KALMAN VS ALPHA-BETA (REAL PROJECT SOURCE)', DARK, 2)
    c.text(30, 46, 'RED = FILTER OUTPUT (AFTER FIX)   GRAY = RAW MEASUREMENT / BEFORE FIX   GREEN = TRUE TARGET', GRAY, 1)

    PW, PH = 700, 250
    X0, Y0 = 110, 130
    DX, DY = 810, 350

    ab_step = read_csv('ab_step_to_zero.csv')
    ab_noise = read_csv('ab_noise.csv')
    ab_clamp = read_csv('ab_clamp_windup.csv')
    kf_zero = read_csv('kf_zero_input.csv')
    kf_noise = read_csv('kf_noise.csv')

    # 1) α-β 阶跃 18°→0°
    draw_panel(c, X0, Y0, PW, PH, '1  ALPHA-BETA: STEP 18 -> 0 DEG',
               [ab_step['t']], [ab_step['angle']], [RED],
               (0, 12), (-1, 19), 'TIME (S)', 'DEG',
               subtitle='DIP TO -0.16 DEG, NO RINGING')

    # 2) α-β 噪声（稳态段）
    t2 = [t for t in ab_noise['t'] if t >= 20]
    a2 = [a for t, a in zip(ab_noise['t'], ab_noise['angle']) if t >= 20]
    draw_panel(c, X0 + DX, Y0, PW, PH, '2  ALPHA-BETA: NOISE, TRUE 0 DEG',
               [t2], [a2], [RED],
               (20, 30), (-0.15, 0.15), 'TIME (S)', 'DEG',
               subtitle='SIGMA IN 0.05 -> OUT STD 0.039 DEG, NEVER EXACT 0')

    # 3) α-β 1s 数据包 vs 真值
    pk = read_csv('ab_packet_vs_truth.csv')
    segx, segy = [], []
    offs = [0.0, 0.25, 0.5, 0.75, 1.0]
    for i, t0 in enumerate(pk['t']):
        pts = [pk['p1'][i], pk['p2'][i], pk['p3'][i], pk['p4'][i]]
        segx += [t0 + o for o in offs] + [None]
        segy += pts + [None]
    truex = [0.2 * k for k in range(0, 51)]
    truey = [8.0 * math.sin(2 * math.pi * 0.5 * t) for t in truex]
    draw_panel(c, X0, Y0 + DY, PW, PH, '3  ALPHA-BETA: 1S PACKET VS TRUTH (0.5HZ)',
               [truex, segx], [truey, segy], [GREEN, RED],
               (0, 9), (-15, 15), 'TIME (S)', 'DEG',
               subtitle='PACKET SWINGS +-12.6 DEG WHILE TRUE SWINGS +-8 DEG')

    # 4) α-β 限幅饱和回摆
    draw_panel(c, X0 + DX, Y0 + DY, PW, PH, '4  ALPHA-BETA: CLAMP WINDUP (25 THEN 5 DEG)',
               [ab_clamp['t'], ab_clamp['t']], [ab_clamp['meas'], ab_clamp['angle']],
               [GRAY, RED],
               (0, 5), (0, 27), 'TIME (S)', 'DEG',
               subtitle='STUCK AT +18 DEG, VELOCITY SATURATED, 39% OVERSHOOT AFTER')

    # 5) 卡尔曼：常值 0 输入
    kz_old = read_csv('kf_cmp_zero_old.csv')
    kz_new = read_csv('kf_cmp_zero_new.csv')
    draw_panel(c, X0, Y0 + 2 * DY, PW, PH, '5  KALMAN: INPUT 0 (FROM 10 DEG) BEFORE/AFTER Q FIX',
               [kz_old['t'], kz_new['t']], [kz_old['angle'], kz_new['angle']],
               [GRAY, RED],
               (0, 30), (-6, 4), 'TIME (S)', 'DEG',
               subtitle='GRAY=BEFORE (182 FLIPS)  RED=AFTER (SETTLES TO 0 IN 0.3S)')

    # 6) 卡尔曼：噪声
    kn_old = read_csv('kf_cmp_noise_old.csv')
    kn_new = read_csv('kf_cmp_noise_new.csv')
    draw_panel(c, X0 + DX, Y0 + 2 * DY, PW, PH, '6  KALMAN: NOISE 0.05 DEG, TRUE 0 - BEFORE/AFTER',
               [kn_old['t'], kn_new['t']], [kn_old['angle'], kn_new['angle']],
               [GRAY, RED],
               (0, 30), (-320, 320), 'TIME (S)', 'DEG',
               subtitle='GRAY=BEFORE (+-315 DEG)   RED=AFTER (|E| < 0.14 DEG)')

    path = os.path.join(OUT, 'filter_test.png')
    c.save(path)
    print('written:', path)


def draw_calibration():
    """σ_a 标定：机动跟踪能力 vs 3s 外推精度 的取舍曲线（x 轴按对数刻度摆放）。"""
    sw = read_csv('kf_sigma_sweep.csv')
    sigmas = sorted(set(sw['sigma']))
    alphas = sorted(set(sw['alpha']))

    def series(alpha, key):
        return [v for a, v in zip(sw['alpha'], sw[key]) if a == alpha]

    W, H = 1720, 800
    c = Canvas(W, H)
    c.text(30, 24, 'SIGMA_A CALIBRATION (KALMAN, 1S PACKET HORIZON 0.25/0.5/0.75/1.0S)', DARK, 2)
    c.text(30, 46, 'X = SIGMA_A (DEG/S^2, LOG SPACING).  THREE CURVES = ALPHA 0.1 (RED) / 0.5 (BLUE) / 1.0 (GREEN)', GRAY, 1)

    PW, PH = 470, 400
    X0, Y0 = 130, 150
    DX = 500
    colors = [RED, BLUE, GREEN]

    panels = [
        ('A  TRACKING RMS (0.5HZ +-8 DEG)', 'sine_rms', (0.0, 5.4),
         'LOWER = BETTER', 'DEG'),
        ('B  1S PACKET RMS, STILL TARGET', 'pkt_rms', (0.0, 5.4),
         'LOWER = LESS PACKET JITTER', 'DEG'),
        ('C  1S PACKET RMS ON THAT MANEUVER', 'sine_pkt_rms', (0.0, 50.0),
         'LOWER = BETTER EXTRAPOLATION', 'DEG'),
    ]

    for pi, (title, key, yr, sub, unit) in enumerate(panels):
        x = X0 + pi * DX
        y = Y0
        w, h = PW, PH
        c.rect(x, y, x + w, y + h, (210, 210, 210))
        c.text(x, y - 24, title, DARK, 2)
        c.text(x + 6, y + 6, sub, GRAY, 1)

        # x 轴：按 log10(σ_a) 均匀摆放
        def mx(i):
            return x + (i + 0.5) * w / len(sigmas)

        def my(v):
            return y + h - (v - yr[0]) / (yr[1] - yr[0]) * h

        for i, sg in enumerate(sigmas):
            gx = mx(i)
            c.line(gx, y, gx, y + h, (238, 238, 238))
            c.text(gx - 16, y + h + 6, ('%g' % sg), GRAY, 1)
        for k in range(5):
            v = yr[0] + (yr[1] - yr[0]) * k / 4.0
            gy = my(v)
            c.line(x, gy, x + w, gy, (238, 238, 238))
            c.text(x - 46, gy - 3, ('%g' % round(v, 2)), GRAY, 1)

        for ai, alpha in enumerate(alphas):
            vals = series(alpha, key)
            px = py = None
            for i, v in enumerate(vals):
                cx, cy = mx(i), my(min(yr[1], v))
                if px is not None:
                    c.line(px, py, cx, cy, colors[ai % 3], 2)
                px, py = cx, cy
            for i, v in enumerate(vals):
                cx, cy = mx(i), my(min(yr[1], v))
                c.rect(cx - 3, cy - 3, cx + 3, cy + 3, colors[ai % 3], fill=True)

        c.text(x + w - 90, y - 24, 'ALPHA 0.1/0.5/1.0', GRAY, 1)
        c.text(x - 46, y - 24, unit, GRAY, 1)
        c.text(x + w - 40, y + h + 6, 'SIGMA_A', GRAY, 1)

    c.text(30, H - 60, 'REFERENCE ALPHA-BETA (PRODUCTION PATH): TRACKING RMS 0.047 / STILL PACKET RMS 0.103 / MANEUVER PACKET RMS 6.48 DEG', GRAY, 1)
    c.text(30, H - 44, 'RECOMMENDED (SIGMA_A 4.0, ALPHA 0.5): TRACKING 0.18 / STILL PACKET 0.49 / MANEUVER PACKET 32.0 DEG', GRAY, 1)
    c.text(30, H - 28, 'PANEL C DECIDES IT: THE KALMAN PACKET IS 5X WORSE THAN ALPHA-BETA ON A 0.5HZ MANEUVER', GRAY, 1)

    path = os.path.join(OUT, 'filter_calib.png')
    c.save(path)
    print('written:', path)


def draw_gate():
    """误检门限机理图：几何示意 + 误差对比。"""
    ng = read_csv('pixel_gate_nogate.csv')
    on = read_csv('pixel_gate_on.csv')

    W, H = 1500, 900
    c = Canvas(W, H)
    c.text(30, 24, 'HOW THE GATE KILLS FALSE DETECTION (10 FALSE FRAMES AT +300 PX)', DARK, 2)
    c.text(30, 46, 'SCENARIO: TRUE TARGET CONF 0.8, FALSE TARGET CONF 0.9 (HIGHER) FOR 0.33 S', GRAY, 1)

    # ── 面板 A：几何示意（放大到误检发生的 1.6s 窗口）──
    ax, ay, aw, ah = 120, 120, 1260, 340
    t0, t1 = 2.6, 4.2
    cands = [y for t, y, h in zip(ng['t'], ng['cand_false'], ng['has_false']) if h > 0.5]
    ymin = min(y for t, y in zip(ng['t'], ng['truth']) if t0 <= t <= t1) - 40
    ymax = (max(cands) if cands else ymin + 400) + 40

    def mx(t): return ax + (t - t0) / (t1 - t0) * aw
    def my(v): return ay + ah - (v - ymin) / (ymax - ymin) * ah

    c.rect(ax, ay, ax + aw, ay + ah, (200, 200, 200))
    c.text(ax, ay - 22, 'A  GEOMETRY: PREDICTION GATE VS CANDIDATE DETECTIONS', DARK, 2)
    c.text(ax + 6, ay + 6, 'SHADED = ACCEPT REGION (PRED +- 4 SIGMA)   GREEN = TRUE BOX   RED = FALSE BOX', GRAY, 1)

    # 门限带
    for t, p, hh in zip(on['t'], on['pred'], on['gate_half']):
        if t0 <= t <= t1:
            c.rect(mx(t), my(p - hh), mx(t) + 2, my(p + hh), (200, 216, 242), fill=True)
    # 门限上下边界（画成两条蓝线，便于看清带宽）
    for sign in (+1, -1):
        px = py = None
        for t, p, hh in zip(on['t'], on['pred'], on['gate_half']):
            if t0 <= t <= t1:
                cx, cy = mx(t), my(p + sign * hh)
                if px is not None:
                    c.line(px, py, cx, cy, BLUE)
                px, py = cx, cy
    # 网格与刻度
    for i in range(5):
        v = ymin + (ymax - ymin) * i / 4.0
        c.line(ax, my(v), ax + aw, my(v), (232, 232, 232))
        c.text(ax - 46, my(v) - 3, '%g' % round(v), GRAY, 1)
    for i in range(5):
        t = t0 + (t1 - t0) * i / 4.0
        c.line(mx(t), ay, mx(t), ay + ah, (232, 232, 232))
        c.text(mx(t) - 12, ay + ah + 6, '%.2f' % t, GRAY, 1)

    for series, col, wid in ((ng['out'], GRAY, 2), (on['out'], RED, 2)):
        px = py = None
        for t, v in zip(ng['t'], series):
            if t0 <= t <= t1:
                cx, cy = mx(t), my(v)
                if px is not None:
                    c.line(px, py, cx, cy, col, wid)
                px, py = cx, cy

    px = py = None
    for t, v in zip(ng['t'], ng['truth']):
        if t0 <= t <= t1:
            cx, cy = mx(t), my(v)
            if px is not None:
                c.line(px, py, cx, cy, GREEN, 2)
            px, py = cx, cy

    for t, v, h in zip(ng['t'], ng['cand_false'], ng['has_false']):
        if h > 0.5 and t0 <= t <= t1:
            c.rect(mx(t) - 4, my(v) - 4, mx(t) + 4, my(v) + 4, RED, fill=True)

    # 标注
    if cands:
        c.text(mx(3.35), my(max(cands)) + 8, 'FALSE BOXES  CONF 0.9', RED, 1)
    c.text(mx(3.05), my(ng['out'][150]) - 20, 'NO GATE: DRAGGED TOWARD FALSE BOX', GRAY, 1)
    c.text(mx(3.05), my(on['out'][150]) + 8, 'GATE: STAYS ON TRUE TARGET', RED, 1)
    c.text(mx(3.05), my(on['pred'][95]) + 6, 'GATE BAND +-4 SIGMA', BLUE, 1)

    c.text(ax + aw - 300, ay + ah + 6, 'TIME (S)', GRAY, 1)
    c.text(ax - 46, ay - 22, 'PX', GRAY, 1)

    # ── 面板 B：误差曲线（全程 30s）──
    e_ng = [abs(o - t) for o, t in zip(ng['out'], ng['truth'])]
    e_on = [abs(o - t) for o, t in zip(on['out'], on['truth'])]
    draw_panel(c, 120, 620, 1260, 220, 'B  OUTPUT ERROR: BEFORE (GRAY) VS AFTER (RED)',
               [ng['t'], on['t']], [e_ng, e_on], [GRAY, RED],
               (0, 30), (0, 120), 'TIME (S)', 'PX',
               subtitle='BEFORE: 111 PX SWING, 4.9 S TO RECOVER   AFTER: STAYS AT THE 3.7 PX NOISE FLOOR')

    path = os.path.join(OUT, 'pixel_gate.png')
    c.save(path)
    print('written:', path)


if __name__ == '__main__':
    main()
    draw_calibration()
    draw_gate()
