from __future__ import annotations

from pathlib import Path

from docx import Document
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT, WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


BASE = Path(__file__).resolve().parent
SOURCE = BASE / "source.docx"
OUTPUT = BASE / "14 DD-GF试验系统软件-数据库设计说明_修改版.docx"


def set_cell_shading(cell, fill: str) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = tc_pr.find(qn("w:shd"))
    if shd is None:
        shd = OxmlElement("w:shd")
        tc_pr.append(shd)
    shd.set(qn("w:fill"), fill)


def set_cell_margins(cell, top=70, start=90, bottom=70, end=90) -> None:
    tc = cell._tc
    tc_pr = tc.get_or_add_tcPr()
    tc_mar = tc_pr.first_child_found_in("w:tcMar")
    if tc_mar is None:
        tc_mar = OxmlElement("w:tcMar")
        tc_pr.append(tc_mar)
    for margin, value in (("top", top), ("start", start), ("bottom", bottom), ("end", end)):
        node = tc_mar.find(qn(f"w:{margin}"))
        if node is None:
            node = OxmlElement(f"w:{margin}")
            tc_mar.append(node)
        node.set(qn("w:w"), str(value))
        node.set(qn("w:type"), "dxa")


def set_table_borders(table, color="D9D9D9", size="4") -> None:
    tbl_pr = table._tbl.tblPr
    borders = tbl_pr.first_child_found_in("w:tblBorders")
    if borders is None:
        borders = OxmlElement("w:tblBorders")
        tbl_pr.append(borders)
    for edge in ("top", "left", "bottom", "right", "insideH", "insideV"):
        node = borders.find(qn(f"w:{edge}"))
        if node is None:
            node = OxmlElement(f"w:{edge}")
            borders.append(node)
        node.set(qn("w:val"), "single")
        node.set(qn("w:sz"), size)
        node.set(qn("w:space"), "0")
        node.set(qn("w:color"), color)


def set_repeat_table_header(row) -> None:
    tr_pr = row._tr.get_or_add_trPr()
    header = OxmlElement("w:tblHeader")
    header.set(qn("w:val"), "true")
    tr_pr.append(header)


def set_fixed_layout(table) -> None:
    tbl_pr = table._tbl.tblPr
    layout = tbl_pr.first_child_found_in("w:tblLayout")
    if layout is None:
        layout = OxmlElement("w:tblLayout")
        tbl_pr.append(layout)
    layout.set(qn("w:type"), "fixed")


def set_run_font(run, size=9.0, bold=None) -> None:
    run.font.name = "宋体"
    run._element.get_or_add_rPr().rFonts.set(qn("w:eastAsia"), "宋体")
    run.font.size = Pt(size)
    if bold is not None:
        run.bold = bold
    run.font.color.rgb = RGBColor(0, 0, 0)


def replace_table(
    table,
    headers: list[str],
    rows: list[list[str]],
    widths_cm: list[float],
    alignments: list[WD_ALIGN_PARAGRAPH] | None = None,
    font_size=9.0,
) -> None:
    old_tbl = table._tbl
    parent = old_tbl.getparent()
    new_table = doc.add_table(rows=len(rows) + 1, cols=len(headers))
    new_table.style = "Table Grid"
    parent.replace(old_tbl, new_table._tbl)
    table = new_table

    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    table.autofit = False
    set_fixed_layout(table)
    set_table_borders(table)
    if alignments is None:
        alignments = [WD_ALIGN_PARAGRAPH.LEFT] * len(headers)

    data = [headers, *rows]
    for row_index, row_data in enumerate(data):
        row = table.rows[row_index]
        if row_index == 0:
            set_repeat_table_header(row)
        for column_index, text in enumerate(row_data):
            cell = row.cells[column_index]
            cell.width = Cm(widths_cm[column_index])
            cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
            set_cell_margins(cell)
            set_cell_shading(cell, "F2F2F2" if row_index == 0 else ("FFFFFF" if row_index % 2 else "F7F9FC"))
            paragraph = cell.paragraphs[0]
            paragraph.alignment = alignments[column_index]
            paragraph.paragraph_format.space_before = Pt(0)
            paragraph.paragraph_format.space_after = Pt(0)
            paragraph.paragraph_format.line_spacing = 1.0
            paragraph.clear()
            run = paragraph.add_run(text)
            set_run_font(run, font_size, bold=(row_index == 0))

    for row in table.rows:
        for cell, width in zip(row.cells, widths_cm):
            cell.width = Cm(width)


def set_paragraph_text(paragraph, text: str, size: float | None = None) -> None:
    paragraph.clear()
    run = paragraph.add_run(text)
    run.font.name = "宋体"
    run._element.get_or_add_rPr().rFonts.set(qn("w:eastAsia"), "宋体")
    if size is not None:
        run.font.size = Pt(size)


def set_update_fields(document) -> None:
    settings = document.settings.element
    update_fields = settings.find(qn("w:updateFields"))
    if update_fields is None:
        update_fields = OxmlElement("w:updateFields")
        settings.append(update_fields)
    update_fields.set(qn("w:val"), "true")


doc = Document(SOURCE)
paragraphs = doc.paragraphs

# Keep the cover, approval page, preface structure, reference table, and TOC.
set_paragraph_text(paragraphs[20], "文档版本：V1.1")
set_paragraph_text(paragraphs[29], "本文件依据 GJB 438C-2021《军用软件开发文档通用要求》附录 L 规定的《数据库设计说明》正文格式编写，规定本软件保存数据时的文件组织、字段定义、写入流程、一致性约束、访问方式及异常处理。本文件以当前 V2.5.0 实现为依据，描述的是已经落盘的数据格式，不描述尚未实现的数据库机制。")
set_paragraph_text(paragraphs[32], "本文档所述“数据库”在物理上是软件保存的一组数据文件。当前实现不建立数据库管理系统，也不使用 SQL 数据库；图像、激光和转台数据分别写入独立 CSV 文件，视频经原始码流暂存后转为 MP4。为满足附录 L 的组织要求，本文将每类文件视为一张表、将 CSV 表头中的列视为数据元素。")
set_paragraph_text(paragraphs[34], "文中以“应”“须”表述的为强制性要求，以“宜”“可”表述的为推荐性要求。文中给出的周期、容量和时延数值，除明确标注为接口或合同约束外，均为依据当前代码和典型行宽作出的设计参考。程序实际行为以第 3 章至第 5 章所述的实现流程为准。")
set_paragraph_text(paragraphs[35], "同批文档中的需求条目、接口和单元标识按已批准版本引用；若个别参数不一致，以最新评审确认版本为准。")

# Table 1 records the updated document version.
doc.tables[0].rows[2].cells[3].paragraphs[0].text = "V1.1"
doc.tables[2].rows[7].cells[1].paragraphs[0].text = "V1.1"
for row in doc.tables[0].rows:
    for cell in row.cells:
        for paragraph in cell.paragraphs:
            for run in paragraph.runs:
                set_run_font(run, 10.5)

# 1.1 and 1.2
set_paragraph_text(paragraphs[66], "本文档为 DD/GF 试验系统软件（以下简称“本软件”或“本 CSCI”）的数据库设计说明，规定当前版本在试验过程中保存的图像、激光、转台和视频数据。保存动作由界面中的“保存数据”按钮发起，三路结构化数据可独立启停，视频在任一路保存期间共用一份。本文档的软件、数据存储与文档标识见表 1。")
set_paragraph_text(paragraphs[69], "本软件运行于银河麒麟桌面操作系统 V10 国防版，默认将数据保存到本地目录 /data/savedata。数据目录按当前月、日创建为 M.d 子目录，三类 CSV 文件和 MP4 视频文件均保存在该子目录中。当前实现不连接数据库服务器，不向网络位置直接写入数据文件。")
set_paragraph_text(paragraphs[71], "1. 用途")
set_paragraph_text(paragraphs[72], "保存的数据用于复核图像导引头、激光导引头和三轴转台在试验过程中的状态，并保留导引头视频画面。CSV 文件提供逐帧结构化状态，MP4 文件提供对应时段的画面；两类数据共同支撑事后检查、算法回放和问题定位。")
set_paragraph_text(paragraphs[73], "2. 一般特性")
set_paragraph_text(paragraphs[74], "本软件不采用数据库管理系统（DBMS），保存的数据由四类文件组成：")
set_paragraph_text(paragraphs[75], "a) 图像数据文件：记录图像导引头 B 帧解析后的工程量，一行对应一帧；")
set_paragraph_text(paragraphs[76], "b) 激光数据文件：记录激光导引头接收帧解析后的工程量，一行对应一帧；")
set_paragraph_text(paragraphs[77], "c) 转台数据文件：记录三轴转台周期状态反馈帧，一行对应一帧；")
set_paragraph_text(paragraphs[78], "d) 视频文件：记录当前导引头视频源，停止保存后形成 MP4；若导引头播放器不可用而 CCD 播放器可用，则使用 CCD 视频源。")
set_paragraph_text(paragraphs[79], "三类 CSV 文件分别由对应数据对象的启停状态控制。任一路开始保存时会登记一次视频录制请求，最后一路停止时结束视频录制。记录器位于主线程；图像和激光解析完成信号直接调用记录槽，转台反馈帧通过排队连接写入记录器。")
set_paragraph_text(paragraphs[80], "文件采用追加写入，运行期间不执行随机更新、删除或跨文件事务。CSV 文件的表头使用中文工程量字段，状态和报警字段保存为中文含义；文件内容不包含任务号、通道号或统一的有效性字段。")
set_paragraph_text(paragraphs[81], "3. 本版本保存方案")
set_paragraph_text(paragraphs[82], "本版本采用“按数据来源分文件、固定周期缓冲、独立启停”的保存方式。每路数据在内存中累积约 200 ms 的文本后写入对应 CSV；视频使用 mpv 的 record-file 功能直接记录原始码流为临时 TS，停止录像后由 ffmpeg 以流复制方式转为 MP4。没有新帧到达时，即使已点击开始保存，也可能不产生该路 CSV 文件。")
set_paragraph_text(paragraphs[83], "4. 需方、用户、开发方与保障机构")
set_paragraph_text(paragraphs[84], "需方：甲方；用户：甲方；开发方：哈尔滨工业大学；保障与运维机构：哈尔滨工业大学（现阶段）。数据所有权归使用单位，开发方仅在排故与技术支持时按审批手续接触数据副本。")
set_paragraph_text(paragraphs[85], "5. 运行现场")
set_paragraph_text(paragraphs[86], "数据存储在方舱内部上位机的本地磁盘。现场不接入互联网，软件不提供远程数据服务；数据交换依靠经审批的移动介质或其他受控介质，按使用单位的数据管理规定执行。当前实现不自动生成媒体标签或数据交接清单。")

# 1.3
set_paragraph_text(paragraphs[88], "本文档用于指导保存功能的维护、CSV 数据解析工具开发、测试中的数据完整性检查以及交付后的数据维护。读者可依据第 4 章的字段表解析 CSV，依据第 5 章理解和检查记录器与视频转换流程。")
set_paragraph_text(paragraphs[89], "本文档共 7 章：第 1 章说明标识与保存范围；第 2 章列出引用文档；第 3 章给出文件型存储的全局设计决策；第 4 章给出四类文件的字段和组织；第 5 章说明数据记录器和视频单元的访问方式；第 6 章给出数据与需求的追踪关系；第 7 章为注释。")
set_paragraph_text(paragraphs[90], "本 CSCI 不建立数据库，不引入 DBMS，也不提供 SQL 查询、事务隔离或数据库索引。本文档按 GJB 438C-2021 附录 L 的要素组织：将每个 CSV 文件视为一张表，将表头中的列视为数据元素；对以 DBMS 为前提的条款，明确标注“不适用”，并说明当前文件实现采用的替代措施。")
set_paragraph_text(paragraphs[91], "本文档与《软件设计说明》的分工为：后者说明软件结构和保存功能为何这样设计，本文档说明当前实现实际保存成什么格式。若文档描述与程序行为不一致，以程序实现和本文档第 3 章至第 5 章的核对结果为依据，并应及时更新文档。文中示例均为构造数据。")

# 2
set_paragraph_text(paragraphs[93], "下列文档中的条款通过本文档的引用而成为本文档的条款。凡注日期或版次的引用文件，仅该版本适用；凡不注日期或版次的引用文件，其最新版本适用于本文档。")

# 3
set_paragraph_text(paragraphs[97], "本章按 GJB 438C-2021 附录 L 第 3 章的要求，给出作用于数据保存全局的设计决策。当前实现不存在分布式数据库、数据库并发控制和数据库索引，相应条款标注为不适用，并说明文件型实现的实际处理方式。")
set_paragraph_text(paragraphs[99], "a) 决策内容：本 CSCI 不采用数据库管理系统。数据以三类 CSV 文件保存图像、激光和转台解析结果，以 MP4 文件保存视频。CSV 首行为中文表头，其后每行对应一帧；每路数据独立创建文件，任意一路停止不影响其余路继续保存。")
set_paragraph_text(paragraphs[100], "b) 决策理由：试验数据按来源逐帧产生，保存需求以顺序记录和事后解析为主，不需要多表关联、随机查询或事务回滚；CSV 可直接用文本工具、表格软件和解析脚本处理，便于现场排故。文件型实现也避免在国产操作系统上引入额外数据库服务和长期运维依赖。")
set_paragraph_text(paragraphs[101], "c) 替代措施：数据库的表结构约束由第 4 章固定的列顺序和列名承担；事务一致性由“单路顺序追加、停止时冲刷并关闭”保证；访问控制由操作系统文件权限承担；跨文件关联依靠目录日期、文件名中的开始时刻和行内时间字段完成。")
set_paragraph_text(paragraphs[102], "d) 不适用的条款：附录 L 中关于数据库管理系统名称、版本、SQL 查询、表空间、数据库索引及数据库分布同步的要求均不适用。本软件不依赖 DBMS，也没有主从数据库或分布式存储节点。")

set_paragraph_text(paragraphs[104], "a) 输入：图像 B 帧经解析完成后通过 frameParsed 信号调用 onImageFrame；激光接收帧经解析完成后通过 frameParsed 信号调用 onLaserFrame；转台状态帧从串口工作线程通过排队连接调用 onTurntableFrame。外部程序不得直接写当前保存文件。")
set_paragraph_text(paragraphs[105], "b) 输出：界面通过 savingImage、savingLaser、savingTurntable 和 saving 属性显示各路的保存状态；记录器将 CSV 文本写入本地文件；视频单元生成临时 TS，并在录像结束后转换和删除临时文件。当前界面未显示记录时长、数据量或行数，也未提供独立的按任务导出功能。")
set_paragraph_text(paragraphs[106], "c) 响应与异常：点击开始后，记录器创建 M.d 目录并置位保存状态，CSV 文件在该路第一帧到达时懒创建；每 200 ms 冲刷一次内存缓冲，停止时立即冲刷并关闭。视频转封装异步执行，上一段仍在转换时拒绝开始新的视频录制。当前实现没有磁盘剩余空间门禁，也不自动删除旧数据或自动分卷。")

set_paragraph_text(paragraphs[108], "a) 界面呈现：三类数据使用各自的“保存数据/停止保存”按钮。按钮状态来自对应布尔属性，不读取文件内容。图像、激光和转台可同时保存；最后一路停止后，视频引用计数归零。")
set_paragraph_text(paragraphs[109], "b) CSV 格式：文件使用 UTF-8 编码并带 BOM，换行符为 LF，字段以半角逗号分隔。表头直接使用中文工程量和单位；数值字段使用小数点，浮点工程量统一保留 4 位小数，整数字段按整数文本写入。当前字段内容不包含逗号、换行和双引号，因此写入器不执行引号转义，也不依赖完整的 RFC 4180 引号规则。")
set_paragraph_text(paragraphs[110], "c) 文件命名：保存根目录默认为 /data/savedata，其下按开始保存时的月、日创建 M.d 目录。三类 CSV 分别命名为 imageHHmmss.csv、laserHHmmss.csv、turntableHHmmss.csv；MP4 命名为 videoHHmmss.mp4。HHmmss 是该路开始保存或第一次视频请求的本地时刻。若同一秒内目标名已存在，则依次追加 _1、_2 等后缀，最多尝试 1000 个名称。")
set_paragraph_text(paragraphs[111], "d) 视频格式：视频源由界面中的导引头播放器提供；导引头播放器缺失时回退到 CCD 播放器。录制期间使用 mpv record-file 保存解包后的原始码流为 TS，不执行视频编码转换。最后一路保存停止后，执行 ffmpeg -c copy -movflags +faststart，将 TS 转为 MP4。")
set_paragraph_text(paragraphs[112], "e) 字段命名：CSV 表头直接使用中文名称并包含单位，列顺序由记录器中的表头和行写入代码共同确定。状态、报警和指令提示保存为中文文本；未映射的状态写为“未知(0xNN)”，便于在不修改字段结构的情况下保留原始码值。")

set_paragraph_text(paragraphs[114], "a) 时间来源：图像和激光行优先保存设备返回时标对应的北京时间；返回时标无效或为零时，保存本机系统时间。转台行保存转台协议给出的当日毫秒时间。三类文件没有统一的整数时间戳字段，也不保存日期；具体格式见第 4 章。")
set_paragraph_text(paragraphs[115], "b) 帧有效性：记录入口只由通信层成功解析一帧后触发，校验失败的帧不会进入 CSV。当前 CSV 没有逐行 valid 标志，也没有统一的全局记录序号；转台文件另有从 1 开始递增的“序号”，仅用于本次转台保存的连续性核对。")
set_paragraph_text(paragraphs[116], "c) 文件级完整性：正常文件应包含且仅包含一行表头，其后每行的逗号分隔字段数与表头一致。正常停止会冲刷缓冲并关闭文件；程序异常终止或断电时，最多可能丢失尚未 flush 的缓冲数据。当前 QFile 只调用 flush，不调用 fsync，操作系统缓存中的尾部数据仍可能受突然断电阻断。")
set_paragraph_text(paragraphs[117], "d) 跨文件对应：三类 CSV 和视频分别记录各自的开始时刻，不生成任务清单、会话索引或视频索引。事后关联时，应先按 M.d 目录筛选日期，再结合文件名中的开始时刻、CSV 行内时间和视频内容确定同一时段。设备返回时标与转台毫秒时间来源不同，当前实现不能保证跨来源的毫秒级同步。")
set_paragraph_text(paragraphs[118], "e) 格式演进：CSV 列以表头名称为准，读取工具不应只按固定列序号解析。当前文件没有独立格式版本号；新增或更名字段必须同步修改表头、写入代码和本文档。状态未知值以原始十六进制写入文本，不改变该列的字符串类型。")

set_paragraph_text(paragraphs[120], "a) 写入模型：记录器运行在主线程，没有独立记录线程、环形队列或入队错误码。各帧先格式化为一行文本并追加到该路 SourceState 的内存 buffer；只要任一路处于保存状态，共享时长为 200 ms 的粗粒度定时器就周期调用 flushAll，把三路缓冲写入磁盘。")
set_paragraph_text(paragraphs[121], "b) 容量估算：按当前典型行宽估算，图像约 7 KiB/s（50 Hz），激光约 6.7 KiB/s（50 Hz），转台约 33 KiB/s（200 Hz），三路合计约 47 KiB/s，连续保存 1 h 约 165 MiB。实际值随数值位数、中文状态长度和实际到达频率变化。视频占用取决于源编码码率，通常显著高于结构化数据。")
set_paragraph_text(paragraphs[122], "c) 空间管理：当前实现不预先检查剩余磁盘空间，不设置预警门限或自动停止阈值，也不按容量或时长分卷。磁盘不可写时由 QFile 写入失败并产生 errorOccurred 信号；文件打开失败后该路进入 openFailed 状态，本次保存期间不再重试。")
set_paragraph_text(paragraphs[123], "d) 优化取舍：文件按来源拆分，避免不同帧率和不同到达条件互相阻塞；使用 200 ms 批量写减少每次写盘调用。该实现以顺序记录和低改造量为目标，没有随机查询索引、自动归档或多个会话的合并表。")

set_paragraph_text(paragraphs[125], "a) 密级标注：数据文件的密级由随包介质标签和交付清单管理，文件名与 CSV 内容均不写密级。当前实现不根据登录角色生成不同密级的数据文件。")
set_paragraph_text(paragraphs[126], "b) 访问控制：在 UNIX 类平台上，记录器将新建目录权限收紧为属主读写执行（0700），将 CSV 和成功转换的 MP4 权限收紧为属主读写（0600）。权限设置失败会使本次目录或文件保存失败；Windows 构建中不执行这两类权限限制。")
set_paragraph_text(paragraphs[127], "c) 敏感字段：CSV、TS 和 MP4 不保存登录口令或密钥。登录校验使用程序内置的盐和 HMAC-SHA256 摘要，明文口令不写入保存文件。图像、激光和转台的状态数据按任务整体密级管理，当前实现不对单个字段加密。")
set_paragraph_text(paragraphs[128], "d) 运行日志：LogManager 仅在内存中保留最多 500 条界面日志，并对高频日志按 200 ms 节流；重启程序后日志丢失，不生成独立审计文件。视频录制因换源或重连中断时，记录器通过 videoRecordInterrupted 将提示写入该内存日志。")
set_paragraph_text(paragraphs[129], "e) 备份：记录器不执行自动备份、复制或同步。备份、归档和介质制作由使用单位按数据管理规定实施。视频转封装成功后删除临时 TS；转换失败时保留 TS，以便后续人工处理。")
set_paragraph_text(paragraphs[130], "f) 恢复：已关闭且结构完整的 CSV 可直接解析；末行字段数不符时应跳过该行。QFile flush 失败时，尚未写入的缓冲会被清空并产生错误信号。视频转换只有在 ffmpeg 正常退出、MP4 存在且大小大于零时才删除 TS；软件不自动修复损坏的 CSV 或 MP4，也不改写原文件。")

set_paragraph_text(paragraphs[132], "a) 分布：数据全部位于本机保存根目录及其 M.d 子目录，不设置主从副本、网络共享目录或多机同步。保存根目录和 ffmpeg 路径是记录器属性，当前界面未提供修改入口。")
set_paragraph_text(paragraphs[133], "b) 维护：文件由记录器自动创建、追加、冲刷和关闭；当前实现不执行容量分卷、过期删除或压缩。每次重新开始某一路时创建新的文件名；同名文件已存在时使用数字后缀，避免覆盖。")
set_paragraph_text(paragraphs[134], "c) 生命周期：数据按试验任务归档，保存期限和执行销毁的审批流程按使用单位数据管理规定执行。软件不自动判定任务边界，M.d 目录可能包含同日多次保存操作产生的文件。")

# 4
set_paragraph_text(paragraphs[136], "本章给出四类文件的详细设计。三类 CSV 均采用“一行中文表头加若干数据行”的结构，一行对应一帧；视频文件不含字段定义。与旧版按统一通道宽表保存的设计不同，当前实现按数据来源拆分文件，文件内不另设 channel、rec_seq 或 valid 列。")
set_paragraph_text(paragraphs[139], "CSV 文件由每路第一帧触发创建，创建后先写带 UTF-8 BOM 的中文表头，再把数据行写入 200 ms 内存缓冲。下面仅以转台文件前四列为例说明行结构；实际文件的完整列数见后续字段表。")
set_paragraph_text(paragraphs[140], "采样率(Hz),序号,转台毫秒时间,指令提示\n200,1,43210000,20ms跟踪", 9.0)
set_paragraph_text(paragraphs[141], "图像、激光和转台文件之间没有公共的通道号、记录序号和有效性列。读取工具应先读取表头，再按当前文件对应的字段表解析；不能把三类文件视为同一宽表。")
set_paragraph_text(paragraphs[142], "记录器默认参数、视频转换参数和内存日志参数不形成配置文件，其当前值和处理方式见 4.5 条。")

# 4.1 Laser
set_paragraph_text(paragraphs[144], "激光数据文件记录激光导引头接收帧解析后的工程量，来源于激光串口通信单元。文件在本次保存收到第一帧后创建，之后每成功解析一帧追加一行。角度和角速度字段按解析后的工程量保存为 4 位小数，不再保存原始整数码值。")
set_paragraph_text(paragraphs[147], "增益状态按协议映射为“起控1级”至“起控5级”；映射表之外的值保存为“未知(0xNN)”。时标精度按相邻两组有效返回时标之差减去 20000 μs 计算，首帧有效时标没有相邻值，因此该单元格为空。")

# 4.2 Image
set_paragraph_text(paragraphs[149], "图像数据文件记录图像导引头 B 帧解析结果，当前实现对应一个图像导引头数据对象和一个保存文件，不使用通道 2、3、4 表示三台设备。文件在收到第一帧 B 帧后创建，每成功解析一帧追加一行。")
# 4.3 and 4.4 body paragraphs. Headings at paragraph indices 152 and 156 are preserved.
set_paragraph_text(paragraphs[153], "转台数据文件记录三轴转台周期状态反馈帧。文件在本次保存收到第一帧后创建，每帧一行。三轴“理想轨迹角度”不直接来自协议，而是记录器按“实测角度加控制偏差”计算后写入；报警、状态和指令提示均转换为中文文本。")
paragraphs[153].paragraph_format.keep_together = True
set_paragraph_text(paragraphs[157], "视频文件记录当前选择的一条导引头视频流。视频不保存每一帧的结构化字段，本节规定录制、文件名、转换和中断处理。图像、激光和转台三路共用同一个视频引用计数，因此多路同时保存只生成一份录像。")

# 4.5 Runtime state
set_paragraph_text(paragraphs[160], "4.5  运行参数（内存）")
set_paragraph_text(paragraphs[161], "以下运行参数和内存状态不形成独立数据文件。当前程序中的默认值如下，修改代码或通过属性设置后才会改变：a) 保存路径：saveDir 默认为 /data/savedata；每次保存另建月、日目录 M.d。ffmpegPath 默认为 ffmpeg，程序按可执行文件搜索路径启动转换进程。")
set_paragraph_text(paragraphs[162], "b) 记录参数：图像和激光标称采样率均为 50 Hz，转台标称采样率为 200 Hz；三路共用 200 ms 粗粒度定时器冲刷缓冲。单路文件名后缀查找上限为 1000，退出时等待视频转换的默认上限为 60000 ms。")
set_paragraph_text(paragraphs[163], "c) 内存状态：每路保存上下文包含 saving、目录、开始时刻、文件路径、openFailed、QFile 和文本缓冲；转台另有序号。LogManager 最多保留 500 条内存日志。以上状态均在进程退出时丢失，不生成配置或会话文件。")

# 5
set_paragraph_text(paragraphs[165], "本章按 GJB 438C-2021 附录 L 第 5 章的要求，给出与数据访问和操纵直接相关的软件单元设计。数据记录器和视频录制接口负责实际文件访问；本章描述当前代码提供的调用关系、线程方式和异常处理。")
set_paragraph_text(paragraphs[167], "a) 职责：DataRecorder 接收图像、激光和转台帧，按第 4 章字段表格式化 CSV 行，管理三路独立保存状态和 200 ms 内存缓冲，登记视频录制请求，并在停止时完成冲刷、关闭和视频收尾。")
set_paragraph_text(paragraphs[168], "b) 数据访问方式：每路保存上下文 SourceState 维护目录、开始时刻、路径、文件句柄、缓冲和打开失败标志。第一帧到达时调用 ensureSourceFile 懒创建文件；后续帧写入该路缓冲。定时器到期后 flushAll 依次冲刷三路，停止某一路时立即冲刷并关闭该文件。")
set_paragraph_text(paragraphs[169], "c) 对外接口：")
set_paragraph_text(paragraphs[170], "1) startImageSave()、startLaserSave()、startTurntableSave()：创建 M.d 目录、置位该路保存状态并登记一次视频请求；成功返回 true，重复启动或目录创建失败返回 false；")
set_paragraph_text(paragraphs[171], "2) stopImageSave()、stopLaserSave()、stopTurntableSave()：冲刷并关闭对应 CSV，释放一次视频请求；某一路停止不影响其他路；")
set_paragraph_text(paragraphs[172], "3) onImageFrame()、onLaserFrame()、onTurntableFrame()：接收解析完成的数据并写内存缓冲；前两者为直接调用，转台槽通过排队连接接收状态帧；")
set_paragraph_text(paragraphs[173], "4) stopAll()：按转台、激光、图像顺序停止三路，并兜底将视频请求计数清零；waitForVideoConversion(timeoutMs)：程序退出时等待 ffmpeg 转封装完成，默认最多 60 s；")
set_paragraph_text(paragraphs[174], "5) status 类属性：savingImage、savingLaser、savingTurntable 和 saving 可由界面读取；errorOccurred、videoConvertFinished、videoRecordInterrupted 以信号方式通知外部。")
set_paragraph_text(paragraphs[175], "DataRecorder 不提供数据库式 open/write/flush/close 句柄，也不提供查询、事务、计数、剩余空间或丢帧统计接口。图像和激光的 frameParsed 信号在它们的数据对象上发出，记录器使用直接连接，以确保读取的是当前帧刚解析完成的值。")
set_paragraph_text(paragraphs[176], "d) 状态与异常：文件或目录创建失败时发出 errorOccurred；CSV 文件打开失败后置位 openFailed，本次保存期间不再每帧重试；普通写入失败时保留尚未写入的缓冲，等待下一次冲刷；QFile flush 失败时关闭文件并清空剩余缓冲。视频转换失败时保留 TS 并发出失败信号。")

set_paragraph_text(paragraphs[178], "a) 职责：VlcVideoItem 通过 mpv 的 record-file 属性把当前视频源原始码流写入临时 TS；DataRecorder 按保存请求计数决定何时开始和停止录像，并在最后一路停止后调用 ffmpeg 转封装。")
set_paragraph_text(paragraphs[179], "b) 数据访问方式：startRecord(path) 设置 mpv record-file；stopRecord() 将 record-file 置空并结束录制。DataRecorder 在第一次保存请求时创建 videoHHmmss.ts，在计数归零后检查 TS 是否存在并启动 ffmpeg。转换命令为 -y -i <TS> -c copy -movflags +faststart <MP4>。")
set_paragraph_text(paragraphs[180], "c) 对外接口：记录器通过 startVideoRecord、stopVideoRecord、startRemux 和 waitForVideoConversion 完成视频生命周期管理；视频播放器通过 recordingChanged 通知录制状态变化。没有视频播放器或视频源未产生数据时，CSV 仍可保存，但不产生视频文件。")
set_paragraph_text(paragraphs[181], "d) 异常处理：保存期间视频源切换或重连会触发 stopRecord；记录器不再续录本次视频，并对中断前已形成的 TS 立即尝试转封装，同时发出 videoRecordInterrupted，界面日志记录“视频源已切换或重连”。ffmpeg 未能启动、退出失败或未生成非空 MP4 时保留 TS；转换成功并确认 MP4 非空后才删除 TS。")

# 6 Traceability
set_paragraph_text(paragraphs[183], "本章按附录 L 第 6 章的要求给出双向追踪：正向确认每个数据文件都有明确用途，反向确认与数据保存有关的需求能够找到承载文件和记录接口。")
set_paragraph_text(paragraphs[186], "需求侧的反向说明：SRS 3.5 条“内部数据需求”规定数据结构由设计阶段确定，当前结论对应第 4 章的三类 CSV 字段表和视频组织；SRS 3.6 条“适应性需求”涉及的路径和运行参数当前由程序属性或代码常量提供，未实现持久化配置。字段或接口变更时，应同步更新第 4 章、第 5 章和本章的追踪关系。")

# 7
set_paragraph_text(paragraphs[192], "a) 本文档依据 GJB 438C-2021 附录 L 规定的《数据库设计说明》正文格式编制。第 3 章对应“数据库级设计决策”，第 4 章对应“数据库详细设计”，第 5 章对应“用于数据库访问或操纵的软件单元的详细设计”，第 6 章对应“需求可追踪性”，第 7 章对应“注释”。")
set_paragraph_text(paragraphs[193], "b) 本文档将当前文件型存储按附录 L 的要素展开，但不虚构 DBMS、表空间、SQL 接口、数据库索引或事务机制。文件格式、字段顺序、200 ms 缓冲、独立启停、共享视频和 TS 转 MP4 的流程均按当前项目代码修改，使设计说明与实际保存行为保持一致。")

# Data-file overview
replace_table(
    doc.tables[4],
    ["序号", "文件类别", "实际内容", "形态", "写入时机", "文件命名"],
    [
        ["1", "图像数据文件", "图像导引头 B 帧解析后的工程量", "CSV 文本", "该路保存后收到第一帧 B 帧起", "imageHHmmss.csv"],
        ["2", "激光数据文件", "激光导引头接收帧解析后的工程量", "CSV 文本", "该路保存后收到第一帧起", "laserHHmmss.csv"],
        ["3", "转台数据文件", "三轴状态、角度、控制偏差与指令提示", "CSV 文本", "该路保存后收到第一帧状态帧起", "turntableHHmmss.csv"],
        ["4", "视频文件", "当前导引头视频源原始码流", "MP4（录制时暂存 TS）", "任一路开始保存时录，最后一路停止时转换", "videoHHmmss.mp4"],
    ],
    [0.9, 1.9, 3.8, 1.9, 2.4, 3.7],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.6,
)

laser_fields = [
    ["1", "时间", "该激光帧的记录时间。", "字符串；优先为设备返回时标对应的北京时间 HH:mm:ss.ffffff，否则为本机系统时间 HH:mm:ss.zzz。"],
    ["2", "采样率(Hz)", "激光数据的标称采样率。", "固定整数 50，对应 20 ms。"],
    ["3", "光轴方位角(°)", "解析后的光轴方位角。", "4 位小数。"],
    ["4", "光轴俯仰角(°)", "解析后的光轴俯仰角。", "4 位小数。"],
    ["5", "方位陀螺输出角速度(°/s)", "方位陀螺输出角速度。", "4 位小数。"],
    ["6", "俯仰陀螺输出角速度(°/s)", "俯仰陀螺输出角速度。", "4 位小数。"],
    ["7", "方位视线角速度(°/s)", "方位视线角速度。", "4 位小数。"],
    ["8", "俯仰视线角速度(°/s)", "俯仰视线角速度。", "4 位小数。"],
    ["9", "方位偏差角(°)", "方位偏差角。", "4 位小数。"],
    ["10", "俯仰偏差角(°)", "俯仰偏差角。", "4 位小数。"],
    ["11", "激光周期(ms)", "协议时间单位换算后的激光周期。", "4 位小数。"],
    ["12", "增益状态", "激光增益起控状态。", "起控1级、起控2级、起控3级、起控4级、起控5级；其他值为未知(0xNN)。"],
    ["13", "第一象限能量强度", "第一象限能量强度。", "4 位小数。"],
    ["14", "第二象限能量强度", "第二象限能量强度。", "4 位小数。"],
    ["15", "第三象限能量强度", "第三象限能量强度。", "4 位小数。"],
    ["16", "第四象限能量强度", "第四象限能量强度。", "4 位小数。"],
    ["17", "时标精度(us)", "相邻两组有效返回时标间隔相对 20000 μs 的偏差。", "有符号整数；首帧或返回时标无效时为空。"],
]
replace_table(
    doc.tables[5],
    ["序号", "字段名", "含义", "格式与写入规则"],
    laser_fields,
    [1.0, 4.3, 4.1, 5.2],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.6,
)

image_fields = [
    ["1", "时间", "该图像 B 帧的记录时间。", "字符串；优先为 B 帧返回时标对应的北京时间 HH:mm:ss.ffffff，否则为本机系统时间 HH:mm:ss.zzz。"],
    ["2", "采样率(Hz)", "图像数据标称采样率。", "固定整数 50，对应 20 ms。"],
    ["3", "B帧流水号", "图像导引头 B 帧流水号。", "整数，按协议解析值写入。"],
    ["4", "当前工作通道", "当前电视或红外工作通道。", "电视、红外；其他值为未知(0xNN)。"],
    ["5", "俯仰视线角速度(°/s)", "俯仰视线角速度。", "4 位小数。"],
    ["6", "偏航视线角速度(°/s)", "偏航视线角速度。", "4 位小数。"],
    ["7", "光学工作状态", "导引头光学工作状态。", "搜索状态（预置状态）、跟踪状态、框架角电锁零位状态、记忆状态、解锁状态；其他值为未知(0xNN)。"],
    ["8", "俯仰框架角(°)", "俯仰框架角。", "4 位小数。"],
    ["9", "偏航框架角(°)", "偏航框架角。", "4 位小数。"],
    ["10", "方位偏差像素", "目标方位偏差像素。", "整数。"],
    ["11", "俯仰偏差像素", "目标俯仰偏差像素。", "整数。"],
    ["12", "俯仰陀螺(°/s)", "俯仰陀螺输出。", "4 位小数。"],
    ["13", "偏航陀螺(°/s)", "偏航陀螺输出。", "4 位小数。"],
    ["14", "跟踪状态", "导引头总体跟踪状态。", "默认、搜索中、目标丢失、目标锁定、记忆状态；其他值为未知(0xNN)。"],
    ["15", "跟踪器状态", "图像跟踪器工作状态。", "空闲状态、跟踪状态、识别状态、匹配状态、仅识别状态；其他值为未知(0xNN)。"],
    ["16", "红外帧编号", "红外通道帧编号。", "整数。"],
    ["17", "电视帧编号", "电视通道帧编号。", "整数。"],
    ["18", "时标精度(us)", "相邻两组有效 B 帧返回时标间隔相对 20000 μs 的偏差。", "有符号整数；首帧或返回时标无效时为空。"],
]
replace_table(
    doc.tables[6],
    ["序号", "字段名", "含义", "格式与写入规则"],
    image_fields,
    [1.0, 4.3, 4.1, 5.2],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.6,
)

status_text = "空闲、伺服、回零执行中、位置执行中、速率执行中、速率稳定、摇摆执行中、摇摆稳定、3秒跟踪模式、250ms跟踪模式、20ms跟踪模式、5ms跟踪模式、停车执行中；其他值为未知(0xN)。"
alarm_text = "无、驱动器报警、伺服超差报警、正向限位报警、逆向限位报警、时钟同步报警、初始化信息报警、限位开关同时导通、编码器数据故障报警、瞬态电流报警、连续电流报警；其他值为未知(0xN)。"
turntable_fields = [
    ["1", "采样率(Hz)", "转台状态反馈标称采样率。", "固定整数 200，对应 5 ms。"],
    ["2", "序号", "本次转台保存内递增的记录序号。", "从 1 开始逐行加一；每路重新开始保存时复位。"],
    ["3", "转台毫秒时间", "状态反馈帧给出的当日毫秒时间。", "整数，协议范围 0～14399999。"],
    ["4", "指令提示", "反馈帧携带的最近有效指令。", "无、使能、停车、回零、位置、速率、摇摆、3秒跟踪、250ms跟踪、20ms跟踪、5ms跟踪、时间设置、跟踪修正、复位；其他值为未知(0xNN)。"],
    ["5", "内框报警", "内框报警状态。", alarm_text],
    ["6", "内框状态", "内框运行状态。", status_text],
    ["7", "内框角度(°)", "内框实测角度。", "4 位小数，协议原始单位为 0.0001°。"],
    ["8", "内框理想轨迹角度(°)", "由内框实测角度加控制偏差得到。", "4 位小数；由记录器计算，不是协议独立字段。"],
    ["9", "内框控制偏差(°)", "内框控制偏差。", "4 位小数，协议原始单位为 0.0001°。"],
    ["10", "中框报警", "中框报警状态。", alarm_text],
    ["11", "中框状态", "中框运行状态。", status_text],
    ["12", "中框角度(°)", "中框实测角度。", "4 位小数。"],
    ["13", "中框理想轨迹角度(°)", "由中框实测角度加控制偏差得到。", "4 位小数；由记录器计算。"],
    ["14", "中框控制偏差(°)", "中框控制偏差。", "4 位小数。"],
    ["15", "外框报警", "外框报警状态。", alarm_text],
    ["16", "外框状态", "外框运行状态。", status_text],
    ["17", "外框角度(°)", "外框实测角度。", "4 位小数。"],
    ["18", "外框理想轨迹角度(°)", "由外框实测角度加控制偏差得到。", "4 位小数；由记录器计算。"],
    ["19", "外框控制偏差(°)", "外框控制偏差。", "4 位小数。"],
    ["20", "秒脉冲", "状态帧的秒脉冲标志。", "有、无。"],
]
replace_table(
    doc.tables[7],
    ["序号", "字段名", "含义", "格式与写入规则"],
    turntable_fields,
    [1.0, 4.2, 4.1, 5.3],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.5,
)

replace_table(
    doc.tables[8],
    ["项目", "当前实现", "说明"],
    [
        ["录制源", "导引头播放器；缺失时回退到 CCD 播放器", "仅录制当前注入 DataRecorder 的一个视频播放器"],
        ["启动与停止", "任一路开始保存时启动，最后一路停止时结束", "三路共用视频请求计数，只生成一份录像"],
        ["暂存格式", "mpv record-file 写入原始码流 TS", "不转码，按源流内容写盘"],
        ["最终格式", "ffmpeg -c copy -movflags +faststart 转为 MP4", "成功且 MP4 非空后删除 TS，否则保留 TS"],
        ["文件命名", "videoHHmmss.mp4", "HHmmss 为第一次视频请求时刻；重名时追加 _1、_2 等后缀"],
        ["分卷", "当前实现不分卷", "录像持续至最后一路停止，或因换源/重连中断"],
        ["中断处理", "立即停止本次录像并转换已录部分", "不续录旧文件；发出 videoRecordInterrupted"],
        ["完整性判据", "ffmpeg 正常退出且 MP4 存在、大小大于零", "转换失败时保留临时 TS 供人工处理"],
    ],
    [3.0, 6.6, 5.0],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)
for row in doc.tables[8].rows:
    for cell in row.cells:
        set_cell_margins(cell, top=50, start=90, bottom=50, end=90)

replace_table(
    doc.tables[9],
    ["数据文件", "支撑的需求 / 条款", "实现中的承载接口"],
    [
        ["图像数据文件", "REQ-FUN-002、REQ-FUN-003、REQ-FUN-012", "ImageData::frameParsed → DataRecorder::onImageFrame"],
        ["激光数据文件", "REQ-FUN-002、REQ-FUN-003、REQ-FUN-012", "LaserData::frameParsed → DataRecorder::onLaserFrame"],
        ["转台数据文件", "REQ-FUN-005、REQ-FUN-006、REQ-FUN-008、013、014", "requpdateframe → DataRecorder::onTurntableFrame"],
        ["视频文件", "REQ-FUN-007；SRS 3.2.3、3.2.7", "DataRecorder 视频请求计数、VlcVideoItem、ffmpeg"],
    ],
    [3.0, 5.6, 6.0],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    9.0,
)

replace_table(
    doc.tables[10],
    ["术语 / 缩略语", "全称或含义", "在本文件中的说明"],
    [
        ["DBDD", "数据库设计说明（Database Design Description）", "即本文档"],
        ["DBMS", "数据库管理系统（Database Management System）", "本软件不使用"],
        ["数据元素", "逻辑设计级的数据项或列", "对应 CSV 表头中的一列"],
        ["CSV", "逗号分隔值（Comma-Separated Values）", "本文档中采用 UTF-8 BOM、LF 换行的简单 CSV"],
        ["UTF-8 BOM", "UTF-8 字节顺序标记", "用于提高中文表格软件的编码识别能力"],
        ["B帧", "图像导引头回传的一类反馈帧", "每帧解析后写入图像 CSV"],
        ["TS", "MPEG 传输流（Transport Stream）", "视频录制时的临时原始码流文件"],
        ["MP4", "MPEG-4 容器格式", "ffmpeg 转封装后的视频文件"],
        ["落盘", "把内存缓冲和录制数据写入持久存储", "本文档中包括 QFile flush 与 TS/MP4 写入"],
    ],
    [3.0, 5.8, 5.8],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    9.0,
)

# Preserve heading and caption relationships across page breaks.
for paragraph in paragraphs:
    if paragraph.style.name in {"Heading 1", "Heading 2", "Caption"}:
        paragraph.paragraph_format.keep_with_next = True

doc.core_properties.version = "1.1"
doc.core_properties.title = "DD/GF 试验系统软件 数据库设计说明"
doc.core_properties.subject = "按当前文件型数据保存实现修订的数据库设计说明"
set_update_fields(doc)
doc.save(OUTPUT)
print(OUTPUT)
