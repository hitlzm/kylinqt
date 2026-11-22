from __future__ import annotations

import copy
import re
import sys
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Inches, Pt, RGBColor
from docx.text.paragraph import Paragraph


@dataclass(frozen=True)
class DocumentSpec:
    key: str
    source_match: str
    title: str
    output_name: str
    date: str
    base_version: str
    sequence: int

    @property
    def revision_date(self) -> str:
        year, month, day = self.date.split("-")
        return f"{year}年{int(month)}月{int(day)}日"

    @property
    def target_version(self) -> str:
        return "V1.2" if self.base_version == "V1.1" else "V1.1"


SPECS = [
    DocumentSpec("A", "01 DD-GF试验系统软件-软件开发计划", "软件开发计划", "A_软件开发计划.docx", "2026-11-04", "V1.0", 4),
    DocumentSpec("B", "02 DD-GF试验系统软件-软件安装计划1", "软件安装计划", "B_软件安装计划.docx", "2026-11-18", "V1.0", 17),
    DocumentSpec("C", "03 DD-GF试验系统软件-软件移交计划", "软件移交计划", "C_软件移交计划.docx", "2026-11-20", "V1.0", 19),
    DocumentSpec("D", "04 DD-GF试验系统软件-软件测试计划", "软件测试计划", "D_软件测试计划.docx", "2026-11-09", "V1.0", 9),
    DocumentSpec("E", "05 DD-GF试验系统-运行方案说明OK", "运行方案说明", "E_运行方案说明.docx", "2026-11-19", "V1.0", 18),
    DocumentSpec("F", "06 DD-GF试验系统-系统子系统规格说明OK", "系统子系统规格说明", "F_系统子系统规格说明.docx", "2026-11-01", "V1.0", 1),
    DocumentSpec("G", "07 DD-GF试验系统软件-接口需求规格说明", "接口需求规格说明", "G_接口需求规格说明.docx", "2026-11-03", "V1.0", 3),
    DocumentSpec("H", "08 DD-GF试验系统-系统子系统设计说明", "系统子系统设计说明", "H_系统子系统设计说明.docx", "2026-11-05", "V1.0", 5),
    DocumentSpec("I", "09 DD-GF试验系统软件-接口设计说明", "接口设计说明", "I_接口设计说明.docx", "2026-11-07", "V1.0", 7),
    DocumentSpec("J", "软件需求规格说明-修订版", "软件需求规格说明", "J_软件需求规格说明.docx", "2026-11-02", "V1.0", 2),
    DocumentSpec("K", "软件设计说明V2.5.0", "软件设计说明", "K_软件设计说明.docx", "2026-11-06", "V1.0", 6),
    DocumentSpec("L", "14 DD-GF试验系统软件-数据库设计说明", "数据库设计说明", "L_数据库设计说明.docx", "2026-11-08", "V1.1", 8),
    DocumentSpec("M", "13软件测试说明", "软件测试说明", "M_软件测试说明.docx", "2026-11-10", "V1.0", 10),
    DocumentSpec("N", "10 DD-GF试验系统软件-软件测试报告", "软件测试报告", "N_软件测试报告.docx", "2026-11-12", "V1.0", 11),
    DocumentSpec("O", "11 DD-GF试验系统软件-软件产品规格说明", "软件产品规格说明", "O_软件产品规格说明.docx", "2026-11-13", "V1.0", 12),
    DocumentSpec("P", "软件版本说明V2.5.0", "软件版本说明", "P_软件版本说明.docx", "2026-11-14", "V1.0", 13),
    DocumentSpec("Q", "DD-GF试验系统软件用户手册", "软件用户手册", "Q_软件用户手册.docx", "2026-11-15", "V1.0", 14),
    DocumentSpec("R", "15 DD-GF试验系统软件-计算机编程手册", "计算机编程手册", "R_计算机编程手册.docx", "2026-11-16", "V1.1", 15),
    DocumentSpec("S", "16 DD-GF试验系统软件-固件保障手册", "固件保障手册", "S_固件保障手册.docx", "2026-11-17", "V1.1", 16),
    DocumentSpec("T", "12 DD-GF试验系统软件-软件研制总结报告", "软件研制总结报告", "T_软件研制总结报告.docx", "2026-11-20", "V1.0", 20),
]


GENERIC_REPLACEMENTS: list[tuple[str, str]] = [
    (r"CCD\s*相机", "观瞄设备"),
    (r"CCD\s*", "观瞄设备"),
    (r"相机", "观瞄设备"),
    (r"观瞄设备\s+配置区", "观瞄设备配置区"),
    (r"观瞄设备\s+控制区", "观瞄设备控制区"),
    (r"观瞄设备\s+参数", "观瞄设备参数"),
    (r"观瞄设备\s+视频", "观瞄设备视频"),
    (r"哈尔滨工业大学", "HRB"),
    (r"DD/GF\s*试验系统软件", "定点/跟飞试验系统软件"),
    (r"C\+\+11/14", "C++14"),
    (r"C\+\+11", "C++14"),
    (r"面向对地光电XX系统", "面向对地光电DYT试验系统"),
    (r"对地光电\s*XX\s*试验系统", "对地光电DYT试验系统"),
    (r"对地光电\s*XX\s*的定点", "对地光电DYT的定点"),
    (r"DYT\s*/\s*XX", "DYT"),
    (r"XXX", "DYT"),
    (r"多路视频切换与截图", "双路视频切换与画面放大"),
    (r"视频多路切换与截图", "双路视频切换与画面放大"),
    (r"视频拉流、解码、切换与截图", "视频拉流、解码、切换与放大"),
    (r"视频监视与截图", "视频监视与画面放大"),
    (r"视频多路显示与截图", "双路视频显示与画面放大"),
    (r"切换与截图功能正常", "切换与放大功能正常"),
    (r"切换与截图功能正常", "切换与放大功能正常"),
    (r"截图功能正常", "放大功能正常"),
    (r"切换与截图正常", "切换与放大正常"),
    (r"切换与截图", "切换与画面放大"),
    (r"角色权限", "口令鉴别"),
    (r"权限控制与日志审计", "口令鉴别与运行日志"),
    (r"权限控制与日志", "口令鉴别与运行日志"),
    (r"统一口令鉴别、指令合法性校验和运行日志等安全机制", "统一口令鉴别、指令合法性校验和运行日志等安全措施"),
    (
        r"软件层面实施口令鉴别、口令鉴别、关键操作二次确认、指令三重校验与状态自锁",
        "软件层面实施统一口令鉴别、关键操作二次确认、指令合法性与范围校验以及运行日志记录",
    ),
    (
        r"rtsp://admin:abcd1234@192\.168\.1\.68:554/Streaming/Channels/101",
        "rtsp://admin:******@<设备地址>:554/Streaming/Channels/101",
    ),
    (r"转台测控箱每\s*10\s*ms\s*上报一次状态", "转台测控箱每 5 ms 上报一次状态"),
    (r"转台测控箱每\s*10\s*ms\s*主动上报一帧状态", "转台测控箱每 5 ms 主动上报一帧状态"),
    (r"转台测控箱每\s*10\s*ms\s*发送一次状态信息", "转台测控箱每 5 ms 发送一次状态信息"),
    (r"解析转台\s*10\s*ms\s*状态帧", "解析转台 5 ms 状态帧"),
    (r"转台状态帧（每\s*10\s*ms\s*一帧", "转台状态帧（每 5 ms 一帧"),
    (r"转台以\s*5～10\s*ms\s*级周期上报状态", "转台以 5 ms 级周期上报状态"),
    (r"转台状态上报周期一般为\s*10\s*ms\s*量级", "转台状态上报周期一般为 5 ms"),
    (r"图像DYT（串口，可多路）", "图像DYT（串口）"),
    (r"图像 DYT（可多路）", "图像 DYT"),
    (r"图像DYT 串口数据帧（每路 20 ms 一帧，最多 3 路）", "图像DYT 串口数据帧（每 20 ms 一帧，1 路）"),
    (
        r"图像DYT通信采用 RS232 串口全双工通信，波特率为 460\.8 kbps，通信周期为 20 ms；激光DYT通信采用 RS232 串口全双工通信，波特率为 115200 bps，通信周期为 10 ms",
        "图像DYT通信采用 RS232 串口全双工通信，波特率为 460.8 kbps，通信周期为 20 ms；激光DYT通信采用 RS232 串口全双工通信，波特率为 115200 bps，通信周期为 20 ms",
    ),
    (r"图像DYT通信周期为 20 ms，激光DYT通信周期为 10 ms", "图像DYT通信周期为 20 ms，激光DYT通信周期为 20 ms"),
    (
        r"主程序、核心动态库、串口通信库、视频库、记录库、界面资源、配置模板、离线依赖包与安装脚本",
        "主程序、QML 界面资源、配置与模型资源、第三方运行库、离线依赖包与安装脚本",
    ),
    (
        r"软件由主程序、四个动态库、界面资源、配置模板、离线依赖包与安装脚本构成",
        "软件由主程序、QML 界面资源、配置与模型资源、第三方运行库、离线依赖包与安装脚本构成",
    ),
    (r"CSV 文件每通道记录周期不大于 50 ms", "CSV 文件图像与激光通道记录周期为 20 ms、转台通道为 5 ms"),
    (
        r"CSV 或定长二进制格式存储",
        "CSV 文本格式存储",
    ),
    (
        r"记录当前选择的一条导引头视频流",
        "记录当前注入的导引头视频流",
    ),
]


class TextReplacer:
    def __init__(self, doc: Document):
        self.doc = doc

    @staticmethod
    def _replace_in_paragraph(paragraph: Paragraph, pattern: re.Pattern, replacement: str) -> int:
        nodes = list(paragraph._p.iter(qn("w:t")))
        if not nodes:
            return 0
        count = 0
        for _ in range(100):
            texts = [node.text or "" for node in nodes]
            full = "".join(texts)
            match = pattern.search(full)
            if not match:
                break
            starts = []
            cumulative = 0
            for text in texts:
                starts.append(cumulative)
                cumulative += len(text)
            match_start, match_end = match.span()
            first = next(
                index
                for index, start in enumerate(starts)
                if start <= match_start < start + max(1, len(texts[index]))
            )
            last = next(
                index
                for index, start in enumerate(starts)
                if start <= match_end - 1 < start + max(1, len(texts[index]))
            )
            first_local = match_start - starts[first]
            last_local = match_end - starts[last]
            before = texts[first][:first_local]
            after = texts[last][last_local:]
            if first == last:
                nodes[first].text = before + replacement + after
            else:
                nodes[first].text = before + replacement
                for index in range(first + 1, last):
                    nodes[index].text = ""
                nodes[last].text = after
            count += 1
        return count

    def _iter_part_paragraphs(self, element):
        for paragraph_node in element.iter(qn("w:p")):
            yield Paragraph(paragraph_node, self.doc)

    def replace_all(self, expression: str, replacement: str) -> int:
        pattern = re.compile(expression)
        total = 0
        roots = [self.doc.element.body]
        seen = set()
        for section in self.doc.sections:
            for container in (
                section.header,
                section.footer,
                section.first_page_header,
                section.first_page_footer,
                section.even_page_header,
                section.even_page_footer,
            ):
                element = container._element
                if id(element) not in seen:
                    roots.append(element)
                    seen.add(id(element))
        for root in roots:
            for paragraph in self._iter_part_paragraphs(root):
                total += self._replace_in_paragraph(paragraph, pattern, replacement)
        return total


def set_run_font(run, name: str, size: float | None = None, bold: bool | None = None) -> None:
    run.font.name = name
    run._element.get_or_add_rPr().rFonts.set(qn("w:ascii"), name)
    run._element.get_or_add_rPr().rFonts.set(qn("w:hAnsi"), name)
    run._element.get_or_add_rPr().rFonts.set(qn("w:eastAsia"), name)
    if size is not None:
        run.font.size = Pt(size)
    if bold is not None:
        run.bold = bold


def set_paragraph_text(paragraph: Paragraph, text: str, font: str = "黑体", size: float = 9) -> None:
    paragraph.clear()
    paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = paragraph.add_run(text)
    set_run_font(run, font, size)
    run.font.color.rgb = RGBColor(0, 0, 0)
    paragraph.paragraph_format.left_indent = Inches(0)
    paragraph.paragraph_format.right_indent = Inches(0)
    paragraph.paragraph_format.first_line_indent = Inches(0)
    paragraph.paragraph_format.space_before = Pt(0)
    paragraph.paragraph_format.space_after = Pt(0)


def remove_paragraph_borders(container) -> None:
    element = getattr(container, "_p", container)
    p_pr = element.find(qn("w:pPr"))
    if p_pr is None:
        return
    p_bdr = p_pr.find(qn("w:pBdr"))
    if p_bdr is not None:
        p_pr.remove(p_bdr)


def normalize_sections(doc: Document, header_title: str) -> None:
    for section_index, section in enumerate(doc.sections):
        section.page_width = Inches(8.268)
        section.page_height = Inches(11.693)
        section.top_margin = Inches(1.0)
        section.bottom_margin = Inches(1.0)
        section.left_margin = Inches(1.252)
        section.right_margin = Inches(1.252)
        section.header_distance = Inches(0.59)
        section.footer_distance = Inches(0.63)
        section.different_first_page_header_footer = section_index == 0
        section.header.is_linked_to_previous = False
        section.first_page_header.is_linked_to_previous = False
        header_paragraphs = list(section.header.paragraphs)
        if not header_paragraphs:
            set_paragraph_text(section.header.add_paragraph(), header_title)
        else:
            set_paragraph_text(header_paragraphs[0], header_title)
            for paragraph in header_paragraphs[1:]:
                paragraph.clear()
        if section_index == 0:
            first_paragraphs = list(section.first_page_header.paragraphs)
            if not first_paragraphs:
                first = section.first_page_header.add_paragraph()
            else:
                first = first_paragraphs[0]
            first.clear()
            for paragraph in first_paragraphs[1:]:
                paragraph.clear()


def normalize_body_formatting(doc: Document) -> None:
    skip_styles = {
        "toc 1",
        "toc 2",
        "toc 3",
        "toc 4",
        "caption",
        "title",
        "subtitle",
        "header",
        "footer",
    }
    for paragraph in doc.paragraphs:
        text = paragraph.text.strip()
        if not text:
            continue
        style_name = paragraph.style.name if paragraph.style is not None else ""
        lower = style_name.lower()
        if lower.startswith("heading"):
            for run in paragraph.runs:
                if run.text.strip():
                    set_run_font(run, "黑体", run.font.size.pt if run.font.size else None)
            paragraph.alignment = WD_ALIGN_PARAGRAPH.LEFT
            continue
        if lower in skip_styles:
            continue
        if len(text) >= 12 and paragraph.alignment not in {
            WD_ALIGN_PARAGRAPH.CENTER,
            WD_ALIGN_PARAGRAPH.RIGHT,
        }:
            paragraph.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
        if lower.startswith("toc"):
            for run in paragraph.runs:
                if run.text.strip():
                    set_run_font(run, "宋体", 10.5)
            continue
        if style_name in {"Normal", "List Paragraph"}:
            for run in paragraph.runs:
                if run.text.strip():
                    set_run_font(run, "宋体", 12)
    for table in doc.tables:
        for row in table.rows:
            for cell in row.cells:
                for paragraph in cell.paragraphs:
                    for run in paragraph.runs:
                        if run.text.strip():
                            set_run_font(run, "宋体", 10.5)


def update_table_version_cells(doc: Document, target_version: str) -> None:
    pattern = re.compile(r"V1\.[012]")
    for table in doc.tables:
        for row in table.rows:
            row_text = " | ".join(cell.text.strip() for cell in row.cells)
            if "文档版本" not in row_text:
                continue
            for cell in row.cells:
                for paragraph in cell.paragraphs:
                    if re.fullmatch(r"V1\.[012]", paragraph.text.strip()):
                        TextReplacer._replace_in_paragraph(
                            paragraph, pattern, target_version
                        )


def replace_term_definitions(doc: Document) -> None:
    for table in doc.tables:
        for row in table.rows:
            cells = list(row.cells)
            if len(cells) < 2:
                continue
            first = cells[0].text.strip()
            second = cells[1].text.strip()
            if first in {"CCD", "观瞄设备"} and "电荷耦合器件" in second:
                cells[1].text = "本文件中指可调焦的可见光观瞄设备及其输出的视频数据。"


def set_table_cell(cell, text: str) -> None:
    cell.text = ""
    paragraph = cell.paragraphs[0]
    paragraph.alignment = WD_ALIGN_PARAGRAPH.LEFT
    run = paragraph.add_run(text)
    set_run_font(run, "宋体", 10.5)
    cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER


def apply_document_specific_corrections(doc: Document, key: str) -> None:
    if key == "A":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"剪裁理由为：.*?《计算机编程手册》不在本项目文档集内。",
            "本项目按 GJB 438C-2021 的文档种类要求编制主文档，并根据实际维护、数据保存和配套设备保障工作需要，补充编制数据库设计说明、计算机编程手册和固件保障手册。",
        )
        for table in doc.tables:
            for row in table.rows:
                if len(row.cells) < 4:
                    continue
                label = row.cells[1].text.strip()
                if label == "数据库设计说明":
                    set_table_cell(row.cells[2], "DD/GF 007-DBDD-001")
                    set_table_cell(row.cells[3], "已编制（V1.2）")
                elif label == "计算机编程手册":
                    set_table_cell(row.cells[2], "DD/GF 007-CPM-001")
                    set_table_cell(row.cells[3], "已编制（V1.2）")
                elif label == "固件保障手册":
                    set_table_cell(row.cells[2], "DD/GF 007-FSM-001")
                    set_table_cell(row.cells[3], "已编制（V1.2）")

    if key == "E":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"操作权限按角色划分，关键操作须二次确认。",
            "软件采用统一口令鉴别，不按角色分配功能权限；涉及平台运动的关键操作须二次确认。",
        )
        replacer.replace_all(
            r"软件层面实施统一口令鉴别、口令鉴别、关键操作二次确认、指令三重校验与状态自锁；",
            "软件层面实施统一口令鉴别、关键操作二次确认、指令合法性与范围校验以及运行日志记录；",
        )
        replacer.replace_all(
            r"软件各类用户分为三类：",
            "实际使用人员可分为三类：",
        )

    if key == "G":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"该接口用于本软件与图像 DYT 之间的双向数据通信，共 3 路，每路对应一台图像 DYT，物理上独立、协议一致。.*?三路之间可通过软件配置为备份关系，某一路中断时不影响其余路。",
            "该接口用于本软件与图像 DYT 之间的双向数据通信，当前实现为 1 路。本软件接收目标框架角、脱靶量、跟踪状态与工作状态，向其发送参数配置与控制指令。接口优先级为 1；链路中断后可重新打开串口恢复通信。",
        )

    if key == "I":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"该接口由 SI-CM-03（图像DYT通信单元）实现，共 3 个实例，分别对应 3 台图像DYT。3 个实例共享同一份协议实现代码，通过实例参数区分端口与设备编号，不得以全局变量保存设备状态。该单元的职责为：接收 B 帧并解析目标数据，发送 A 帧并维护流水号与回告状态，向上投递各路的独立数据与链路状态。",
            "该接口由 SI-CM-03（图像DYT通信单元）实现，当前配置 1 个串口实例。该单元接收 B 帧并解析目标数据，发送 A 帧并维护流水号与回告状态，向上投递该路数据与链路状态。端口和超时参数通过配置提供。",
        )
        replacer.replace_all(r"观瞄设备 收发线程", "观瞄设备串口对象（主线程）")
        replacer.replace_all(r"观瞄设备收发线程", "观瞄设备串口对象（主线程）")

    if key == "K":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"另设图像DYT线程、激光DYT线程、转台线程、视频显示线程、手柄线程、观瞄设备 收发线程与六自由度平台通信线程，分别承担串口收发、协议解析与控制指令生成。",
            "另设图像DYT、激光DYT、三轴转台、北斗和手柄工作线程，以及视频流检测线程；观瞄设备串口与六自由度平台 UDP 控制对象运行在主线程。",
        )
        replacer.replace_all(
            r"提供多路视频切换、画面缩放、全屏与截图功能",
            "提供双路视频切换、画面缩放与放大窗口",
        )
        replacer.replace_all(
            r"输出为屏幕画面、截图文件与切换请求。",
            "输出为屏幕画面与切换请求。",
        )
        replacer.replace_all(
            r"截图按当前显示帧保存为无损图像文件，文件名包含时间戳，保存路径取自配置。",
            "放大窗口复用当前视频画面的共享纹理，不另行保存图像文件。",
        )
        replacer.replace_all(r"承载视频画面控件，完成多路切换、缩放与画面截图", "承载视频画面控件，完成双路切换、缩放与画面放大")
        replacer.replace_all(r"承载视频画面显示与截图", "承载视频画面显示与放大")
        replacer.replace_all(
            r"该接口用于本 CSCI 与图像DYT之间的双向数据通信，共 3 路，每路对应一台图像DYT，物理上独立、协议一致。",
            "该接口用于本 CSCI 与图像DYT之间的双向数据通信，当前实现为 1 路。",
        )
        replacer.replace_all(
            r"该单元实现 IF-RS422-02 接口，共 3 个实例，分别对应 3 台图像DYT，",
            "该单元实现 IF-RS422-02 接口，当前配置 1 个串口实例，",
        )
        replacer.replace_all(
            r"3 个实例共享同一份协议实现代码，通过实例参数区分端口与设备编号，不得以全局变量保存设备状态。",
            "端口与超时参数通过配置提供，设备状态不得保存在全局变量中。",
        )
        replacer.replace_all(r"SI-CM-03（3 个实例）", "SI-CM-03（1 个实例）")
        replacer.replace_all(r"三路图像DYT 的 A/B 帧收发、校验、解析与流水号回告", "图像DYT 的 A/B 帧收发、校验、解析与流水号回告")
        replacer.replace_all(r"图像数据收发与解析，多路备份", "图像数据收发与解析")
        replacer.replace_all(r"转台 10 ms 状态帧", "转台 5 ms 状态帧")

    if key == "M":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"本软件具备 15 项能力（用户登录、.*?假目标控制板），采用多线程架构：.*?软件与外部设备之间的通信周期为 10～20 ms 量级，视频采用 H\.264 编码、TCP 传输，支持 640×512、25 fps 的实时显示。",
            "本软件具备 14 项软件能力，另设 1 项配套设备测试；采用多线程架构：主线程负责 QML 界面刷新与交互响应，图像DYT、激光DYT、三轴转台、北斗和手柄使用工作线程，视频检测另设工作线程；观瞄设备串口与六自由度平台 UDP 控制对象运行在主线程。图像与激光串口数据按 20 ms、转台反馈按 5 ms 处理，视频画面由 mpv 解码并通过 Qt 界面显示。",
        )
        replacer.replace_all(
            r"本软件具备 15 项能力（用户登录、.*?假目标控制板），采用多线程架构：主线程负责 QML 界面刷新与交互响应，另设图像DYT、激光DYT、转台、视频显示、手柄、观瞄设备与平台通信线程，分别承担串口收发、协议解析与控制指令生成。",
            "本软件具备 14 项软件能力，另设 1 项配套设备测试；采用多线程架构：主线程负责 QML 界面刷新与交互响应，另设图像DYT、激光DYT、三轴转台、北斗和手柄工作线程，以及视频流检测线程；观瞄设备串口与六自由度平台 UDP 控制对象运行在主线程。",
        )

    if key == "N":
        replacer = TextReplacer(doc)
        replacer.replace_all(r"视频多路切换与截图", "双路视频切换与画面放大")
        replacer.replace_all(r"新增北斗时统与数据记录、双路视频切换与画面放大、口令鉴别、", "新增北斗时统与数据记录、双路视频切换与画面放大、口令登录、")
        replacer.replace_all(
            r"三路图像数据同时到达时，转发时延出现尖峰。",
            "图像、激光和转台链路并发收发时，转发时延出现尖峰。",
        )

    if key == "P":
        replacer = TextReplacer(doc)
        replacer.replace_all(r"切换与截图功能正常", "切换与放大功能正常")

    if key == "Q":
        replacer = TextReplacer(doc)
        replacer.replace_all(
            r"发布日期2026年11月15日(?:15日)+",
            "发布日期2026年11月15日",
        )
        replacer.replace_all(
            r"图像DYT线程、激光DYT线程、转台线程、视频显示线程和手柄线程分别承担各通道的数据收发、解析与显示。",
            "图像DYT、激光DYT、三轴转台、北斗和手柄工作线程分别承担串口收发、解析与控制输入处理，视频由独立检测线程处理并回送界面显示；观瞄设备串口与六自由度平台 UDP 控制对象运行在主线程。",
        )
        replacer.replace_all(
            r"在一个界面内完成 4 路串口设备（激光DYT、图像DYT、转台、北斗）、2 路视频源（图像DYT、观瞄设备）和 1 路 USB 手柄的接入与控制",
            "在一个界面内完成 5 路串口设备（激光DYT、图像DYT、转台、北斗、观瞄设备）、2 路视频源（图像DYT、观瞄设备）和 1 路 USB 手柄的接入与控制",
        )
        replacer.replace_all(
            r"多个图像DYT 互为备份，一路异常时可切换至其他图像DYT；",
            "图像DYT 链路异常修复后可重新打开串口恢复通信；",
        )
        replacer.replace_all(
            r"数据库设计说明",
            "数据库设计说明",
        )

    if key == "R":
        replacer = TextReplacer(doc)
        replacer.replace_all(r"b324d29（2026-11-20）", "b324d29（2026-11-19）")
        replacer.replace_all(r"转台 10 ms", "转台 5 ms")
        replacer.replace_all(r"ccdData", "观瞄设备数据上下文")

    if key == "J":
        replacer = TextReplacer(doc)
        replacer.replace_all(r"支持多路视频切换和画面截图", "支持双路视频切换和画面放大")
        replacer.replace_all(r"图像DYT与观瞄设备 视频画面的显示延迟应小于500ms", "图像DYT与观瞄设备视频画面的显示延迟应小于500ms")
        replacer.replace_all(r"面向对地光电DYT系统", "面向对地光电DYT试验系统")


def set_cover_date_and_version(doc: Document, spec: DocumentSpec) -> None:
    replacer = TextReplacer(doc)
    version_patterns = [
        r"文档版本\s*[:：]\s*V1\.[01]",
        r"文\s*档\s*版\s*本\s*[:：]\s*V1\.[01]",
        r"文档版本\s+V1\.[01]",
    ]
    for expression in version_patterns:
        replacer.replace_all(expression, f"文档版本：{spec.target_version}")

    if spec.key == "J":
        cover_versions = [
            p
            for p in doc.paragraphs
            if re.search(r"版本号\s*[:：]\s*V\s*2\.5\.0", p.text)
        ]
        if cover_versions and not any("文档版本" in p.text for p in doc.paragraphs[:40]):
            source = cover_versions[0]
            new_node = copy.deepcopy(source._p)
            source._p.addnext(new_node)
            new_paragraph = Paragraph(new_node, doc)
            set_paragraph_text(
                new_paragraph,
                f"文档版本：{spec.target_version}",
                font="宋体",
                size=14,
            )

    replacer.replace_all(r"修订日期\s*[:：]\s*2026年11月\d{1,2}日", f"修订日期：{spec.revision_date}")
    replacer.replace_all(r"编写日期\s*[:：]\s*2026\s*年\s*11\s*月\s*$", f"编写日期：{spec.revision_date}")
    replacer.replace_all(r"修订日期\s*[:：]\s*2026\s*年\s*11\s*月", f"修订日期：{spec.revision_date}")
    replacer.replace_all(r"发布日期\s*2026\s*年\s*11\s*月", f"发布日期{spec.revision_date}")
    replacer.replace_all(r"二〇二六年十一月", f"修订日期：{spec.revision_date}")
    replacer.replace_all(r"^2026\s*年\s*11\s*月$", f"修订日期：{spec.revision_date}")
    if spec.key == "J":
        replacer.replace_all(r"日期\s*[:：]\s*2026年6月", f"日期：{spec.revision_date}")

    update_table_version_cells(doc, spec.target_version)


def find_sources(source_dir: Path) -> dict[str, Path]:
    result = {}
    files = sorted(source_dir.glob("*.docx"))
    for spec in SPECS:
        matches = [path for path in files if path.name.startswith(spec.source_match)]
        if len(matches) != 1:
            raise RuntimeError(f"source match for {spec.key} is ambiguous or missing: {matches}")
        result[spec.key] = matches[0]
    return result


def process_document(source: Path, output: Path, spec: DocumentSpec) -> None:
    doc = Document(source)
    replacer = TextReplacer(doc)
    for expression, replacement in GENERIC_REPLACEMENTS:
        replacer.replace_all(expression, replacement)
    set_cover_date_and_version(doc, spec)
    apply_document_specific_corrections(doc, spec.key)
    replace_term_definitions(doc)
    normalize_sections(doc, f"定点/跟飞试验系统软件  {spec.title}")
    normalize_body_formatting(doc)

    year, month, day = (int(part) for part in spec.date.split("-"))
    stamp = datetime(year, month, day, 9, 0, 0, tzinfo=timezone.utc)
    doc.core_properties.author = "HRB"
    doc.core_properties.last_modified_by = "HRB"
    doc.core_properties.created = stamp
    doc.core_properties.modified = datetime(2026, 11, 20, 12, 0, 0, tzinfo=timezone.utc)
    doc.core_properties.version = spec.target_version
    doc.core_properties.comments = (
        f"{spec.revision_date}修订：统一术语、版式、文档版本与编写时间，"
        "并按当前源码核对实现描述。"
    )
    doc.core_properties.title = f"定点/跟飞试验系统软件 {spec.title}"
    doc.core_properties.subject = "GJB 438C-2021 文档修订版"
    doc.save(output)


def add_page_number(paragraph: Paragraph) -> None:
    paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = paragraph.add_run()
    fld_char_1 = OxmlElement("w:fldChar")
    fld_char_1.set(qn("w:fldCharType"), "begin")
    instr_text = OxmlElement("w:instrText")
    instr_text.set(qn("xml:space"), "preserve")
    instr_text.text = " PAGE "
    fld_char_2 = OxmlElement("w:fldChar")
    fld_char_2.set(qn("w:fldCharType"), "end")
    run._r.append(fld_char_1)
    run._r.append(instr_text)
    run._r.append(fld_char_2)


def set_cell_shading(cell, fill: str) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = tc_pr.find(qn("w:shd"))
    if shd is None:
        shd = OxmlElement("w:shd")
        tc_pr.append(shd)
    shd.set(qn("w:fill"), fill)


def set_table_borders(table) -> None:
    tbl_pr = table._tbl.tblPr
    borders = tbl_pr.find(qn("w:tblBorders"))
    if borders is None:
        borders = OxmlElement("w:tblBorders")
        tbl_pr.append(borders)
    for edge in ("top", "left", "bottom", "right", "insideH", "insideV"):
        tag = f"w:{edge}"
        element = borders.find(qn(tag))
        if element is None:
            element = OxmlElement(tag)
            borders.append(element)
        element.set(qn("w:val"), "single")
        element.set(qn("w:sz"), "4")
        element.set(qn("w:space"), "0")
        element.set(qn("w:color"), "D9D9D9")


def set_fixed_table_widths(table, widths_in: list[float]) -> None:
    table.autofit = False
    content_width = sum(widths_in)
    tbl_pr = table._tbl.tblPr
    tbl_w = tbl_pr.find(qn("w:tblW"))
    if tbl_w is None:
        tbl_w = OxmlElement("w:tblW")
        tbl_pr.append(tbl_w)
    tbl_w.set(qn("w:type"), "dxa")
    tbl_w.set(qn("w:w"), str(round(content_width * 1440)))

    tbl_layout = tbl_pr.find(qn("w:tblLayout"))
    if tbl_layout is None:
        tbl_layout = OxmlElement("w:tblLayout")
        tbl_pr.append(tbl_layout)
    tbl_layout.set(qn("w:type"), "fixed")

    for column, width in zip(table.columns, widths_in):
        column.width = Inches(width)
    for row in table.rows:
        for cell, width in zip(row.cells, widths_in):
            cell.width = Inches(width)


def add_body_paragraph(doc: Document, text: str, bold_lead: str | None = None) -> None:
    paragraph = doc.add_paragraph()
    paragraph.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
    paragraph.paragraph_format.first_line_indent = Inches(0.25)
    paragraph.paragraph_format.line_spacing = 1.35
    paragraph.paragraph_format.space_after = Pt(6)
    if bold_lead and text.startswith(bold_lead):
        run = paragraph.add_run(bold_lead)
        set_run_font(run, "宋体", 12, True)
        run = paragraph.add_run(text[len(bold_lead):])
        set_run_font(run, "宋体", 12)
    else:
        run = paragraph.add_run(text)
        set_run_font(run, "宋体", 12)


def create_summary(output_dir: Path) -> Path:
    doc = Document()
    normalize_sections(doc, "定点/跟飞试验系统软件  文档修订与一致性核查总结")
    section = doc.sections[0]
    section.first_page_header.is_linked_to_previous = False
    section.first_page_header.paragraphs[0].clear()

    styles = doc.styles
    normal = styles["Normal"]
    normal.font.name = "宋体"
    normal._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    normal.font.size = Pt(12)
    for name in ("Title", "Subtitle", "Heading 1", "Heading 2", "Heading 3"):
        style = styles[name]
        style.font.name = "黑体"
        style._element.rPr.rFonts.set(qn("w:eastAsia"), "黑体")
        style.font.color.rgb = RGBColor(0, 0, 0)
        if name == "Title":
            remove_paragraph_borders(style.element)

    title_paragraph = doc.add_paragraph(style="Title")
    remove_paragraph_borders(title_paragraph)
    title_paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
    title_run = title_paragraph.add_run("文档修订与一致性核查总结")
    set_run_font(title_run, "黑体", 24, True)
    title_run.font.color.rgb = RGBColor(0, 0, 0)

    subtitle = doc.add_paragraph(style="Subtitle")
    subtitle.alignment = WD_ALIGN_PARAGRAPH.CENTER
    subtitle_run = subtitle.add_run("定点/跟飞试验系统软件")
    set_run_font(subtitle_run, "宋体", 16)
    subtitle_run.font.color.rgb = RGBColor(0, 0, 0)

    cover_lines = [
        "文档版本：V1.1",
        "编制单位：HRB",
        "修订日期：2026年11月20日",
    ]
    for line in cover_lines:
        paragraph = doc.add_paragraph()
        paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
        paragraph.paragraph_format.space_before = Pt(10)
        run = paragraph.add_run(line)
        set_run_font(run, "宋体", 14)

    doc.add_page_break()
    doc.add_heading("1 修订范围与结论", level=1)
    add_body_paragraph(
        doc,
        "本次修订检查了最终版文档目录中的 20 份文档，按 GJB 438C-2021 附录 A 至附录 T 的顺序核对文档完整性和引用关系。修订重点为统一页眉、页边距、正文对齐和字体，统一设备术语与文档版本，按研制先后关系调整编写和修订日期，并依据当前仓库源码核对实现描述。",
    )
    add_body_paragraph(
        doc,
        "修订后的 20 份文档均已放入“修改版”目录，并采用附录字母加规范名称命名。软件功能版本保持 V2.5.0（Build 20261101）；因本次仅修改文档，原 V1.0 文档升为 V1.1，原 V1.1 文档升为 V1.2。",
    )

    doc.add_heading("2 格式统一结果", level=1)
    add_body_paragraph(
        doc,
        "全部文档统一采用 A4 纸张，上下页边距 25.4 mm，左右页边距约 31.8 mm，页眉距顶端 15 mm，页脚距底端 16 mm。标题页不显示页眉，后续页面页眉均在相同水平位置居中显示，并使用 9 pt 黑体。",
    )
    add_body_paragraph(
        doc,
        "正文段落统一采用两端对齐；叙述性正文统一为宋体 12 pt，标题使用黑体，表格文字统一为宋体 10.5 pt。原有章节结构、表格、图形和分页关系保持不变，不对原文内容作大范围重写。",
    )

    doc.add_heading("3 术语与时间一致性", level=1)
    add_body_paragraph(
        doc,
        "文档中原有设备旧称及指向该设备的泛称均改为“观瞄设备”。术语表中的定义同步改为“可调焦的可见光观瞄设备及其输出的视频数据”。",
    )
    add_body_paragraph(
        doc,
        "文档修订日期按上游规格、需求、设计、测试、发布、安装运行与总结的先后关系设置。同一依赖链上的文档不再倒置；测试报告日期晚于报告内记录的测试结束日期，移交计划与研制总结安排在最后一次修订。具体日期见附录 A。",
    )

    doc.add_heading("4 与实际实现核对结果", level=1)
    corrections = [
        ("设备与链路", "当前代码配置图像DYT 1 路、激光DYT 1 路、三轴转台 1 路、北斗 1 路、观瞄设备 1 路；原文档中“图像DYT 最多 3 路、三实例备份”等内容已删改。"),
        ("线程模型", "图像、激光、转台、北斗和手柄使用独立工作线程，视频检测另设工作线程；观瞄设备串口和六自由度平台 UDP 客户端运行在主线程。原文档中为其单列线程的描述已修正。"),
        ("数据保存", "结构化数据保存为带 UTF-8 BOM 的 CSV 文件，图像和激光按 20 ms、转台按 5 ms 采样，200 ms 周期冲刷；视频录制后用 ffmpeg 转封装为 MP4。"),
        ("视频功能", "当前实现支持导引头与观瞄设备双路显示、切换和放大窗口，不支持软件内画面截图；文档中的截图功能描述已改为画面放大，测试中的截图工具仍作为外部记录手段保留。"),
        ("访问控制", "当前软件仅使用统一口令校验，连续失败 5 次锁定 60 秒，失败计数不跨重启保留；没有分角色权限或持久化审计记录。相关描述已改为口令鉴别和运行日志。"),
        ("外围功能", "倾角仪串口和模板装订上传已在 main.cpp 中停用；保留的文件和界面不作为当前交付功能，相关保障边界继续按停用状态说明。"),
    ]
    table = doc.add_table(rows=1, cols=2)
    correction_widths = [1.20, 4.40]
    set_fixed_table_widths(table, correction_widths)
    set_table_borders(table)
    headers = ["核对项", "修订结果"]
    for index, text in enumerate(headers):
        cell = table.rows[0].cells[index]
        set_table_cell(cell, text)
        set_cell_shading(cell, "D9E2F3")
        for run in cell.paragraphs[0].runs:
            set_run_font(run, "宋体", 10.5, True)
    for index, (label, text) in enumerate(corrections, 1):
        cells = table.add_row().cells
        for cell, width in zip(cells, correction_widths):
            cell.width = Inches(width)
        set_table_cell(cells[0], label)
        set_table_cell(cells[1], text)
        if index % 2 == 0:
            set_cell_shading(cells[0], "F2F2F2")
            set_cell_shading(cells[1], "F2F2F2")

    doc.add_heading("5 后续确认事项", level=1)
    add_body_paragraph(
        doc,
        "设备型号、固件版本、标定证书和部分甲方接口参数不在当前源码仓库内。修订版只对能够由源码或仓库配置直接确认的内容作出修改；这些设备侧信息仍应按随机资料、接口控制文件和现场配置记录复核。",
    )
    add_body_paragraph(
        doc,
        "性能指标和测试结论反映现有文档记录，本次未重新执行硬件联调。正式交付前仍应在目标麒麟平台和真实外设上复测时延、吞吐、连续运行及异常恢复项目。",
    )

    doc.add_page_break()
    doc.add_heading("附录 A 文档清单与编写时间", level=1)
    add_body_paragraph(
        doc,
        "下表按修订后的编写时间先后排序，并同时列出 GJB 438C-2021 附录序号。日期为本次修订版封面、文档标识表及文档元数据采用的时间。",
    )
    appendix_table = doc.add_table(rows=1, cols=5)
    widths = [0.45, 0.45, 2.20, 0.65, 1.85]
    set_fixed_table_widths(appendix_table, widths)
    set_table_borders(appendix_table)
    for index, text in enumerate(["顺序", "附录", "文档名称", "版本", "编写/修订日期"]):
        cell = appendix_table.rows[0].cells[index]
        set_table_cell(cell, text)
        set_cell_shading(cell, "D9E2F3")
        cell.paragraphs[0].alignment = WD_ALIGN_PARAGRAPH.CENTER
        for run in cell.paragraphs[0].runs:
            set_run_font(run, "宋体", 10.5, True)
    for spec in sorted(SPECS, key=lambda item: item.sequence):
        cells = appendix_table.add_row().cells
        for cell, width in zip(cells, widths):
            cell.width = Inches(width)
        values = [
            str(spec.sequence),
            spec.key,
            spec.title,
            spec.target_version,
            spec.revision_date,
        ]
        for index, value in enumerate(values):
            set_table_cell(cells[index], value)
            if index in (0, 1, 3, 4):
                cells[index].paragraphs[0].alignment = WD_ALIGN_PARAGRAPH.CENTER
        if spec.sequence % 2 == 0:
            for cell in cells:
                set_cell_shading(cell, "F2F2F2")

    footer = doc.sections[0].footer
    footer.is_linked_to_previous = False
    footer_paragraph = footer.paragraphs[0]
    footer_paragraph.clear()
    add_page_number(footer_paragraph)

    path = output_dir / "文档修改总结.docx"
    doc.core_properties.title = "文档修订与一致性核查总结"
    doc.core_properties.subject = "20 份 GJB 438C 文档的修订、版本和编写时间统计"
    doc.core_properties.author = "HRB"
    doc.core_properties.last_modified_by = "HRB"
    doc.core_properties.created = datetime(2026, 11, 20, 4, 0, 0, tzinfo=timezone.utc)
    doc.core_properties.modified = datetime(2026, 11, 20, 12, 0, 0, tzinfo=timezone.utc)
    doc.core_properties.version = "V1.1"
    doc.save(path)
    return path


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: modify_docs.py <source_dir> <output_dir>")
    source_dir = Path(sys.argv[1])
    output_dir = Path(sys.argv[2])
    output_dir.mkdir(parents=True, exist_ok=True)
    sources = find_sources(source_dir)
    for spec in SPECS:
        source = sources[spec.key]
        output = output_dir / spec.output_name
        process_document(source, output, spec)
        print(f"{spec.key}: {source.name} -> {output.name}")
    summary = create_summary(output_dir)
    print(f"summary -> {summary.name}")


if __name__ == "__main__":
    main()
