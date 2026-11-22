from __future__ import annotations

from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT, WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


def set_cell_shading(cell, fill: str) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = tc_pr.find(qn("w:shd"))
    if shd is None:
        shd = OxmlElement("w:shd")
        tc_pr.append(shd)
    shd.set(qn("w:fill"), fill)


def set_cell_margins(cell, top=70, start=90, bottom=70, end=90) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
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


def set_row_cant_split(row) -> None:
    tr_pr = row._tr.get_or_add_trPr()
    if tr_pr.find(qn("w:cantSplit")) is None:
        cant_split = OxmlElement("w:cantSplit")
        tr_pr.append(cant_split)


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
    document,
    table,
    headers: list[str],
    rows: list[list[str]],
    widths_cm: list[float],
    alignments: list[WD_ALIGN_PARAGRAPH] | None = None,
    font_size=9.0,
    cell_margin=70,
):
    old_tbl = table._tbl
    parent = old_tbl.getparent()
    new_table = document.add_table(rows=len(rows) + 1, cols=len(headers))
    new_table.style = "Table Grid"
    parent.replace(old_tbl, new_table._tbl)
    table = new_table
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    table.autofit = False
    set_fixed_layout(table)
    set_table_borders(table)
    if alignments is None:
        alignments = [WD_ALIGN_PARAGRAPH.LEFT] * len(headers)

    for row_index, row_data in enumerate([headers, *rows]):
        row = table.rows[row_index]
        set_row_cant_split(row)
        if row_index == 0:
            set_repeat_table_header(row)
        for column_index, text in enumerate(row_data):
            cell = row.cells[column_index]
            cell.width = Cm(widths_cm[column_index])
            cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
            set_cell_margins(cell, top=cell_margin, start=90, bottom=cell_margin, end=90)
            set_cell_shading(
                cell,
                "F2F2F2" if row_index == 0 else ("FFFFFF" if row_index % 2 else "F7F9FC"),
            )
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
    return table


def keep_table_together(table) -> None:
    for row in table.rows:
        for cell in row.cells:
            for paragraph in cell.paragraphs:
                paragraph.paragraph_format.keep_with_next = True
    if table.rows:
        for cell in table.rows[-1].cells:
            for paragraph in cell.paragraphs:
                paragraph.paragraph_format.keep_with_next = False


def set_paragraph_text(paragraph, text: str, size: float | None = None) -> None:
    paragraph.clear()
    run = paragraph.add_run(text)
    run.font.name = "宋体"
    run._element.get_or_add_rPr().rFonts.set(qn("w:eastAsia"), "宋体")
    if size is not None:
        run.font.size = Pt(size)


def replace_heading_text(paragraph, text: str) -> None:
    if not paragraph.runs:
        set_paragraph_text(paragraph, text)
        return
    paragraph.runs[0].text = text
    for run in paragraph.runs[1:]:
        run.text = ""


def update_toc_label(paragraph, new_label: str) -> None:
    seen_tab = False
    wrote_label = False
    for run in paragraph._p.xpath(".//w:hyperlink[1]/w:r"):
        if run.xpath(".//w:tab"):
            seen_tab = True
            continue
        if seen_tab:
            continue
        for text_node in run.xpath(".//w:t"):
            if not wrote_label:
                text_node.text = new_label
                wrote_label = True
            else:
                text_node.text = ""


def set_update_fields(document) -> None:
    settings = document.settings.element
    update_fields = settings.find(qn("w:updateFields"))
    if update_fields is None:
        update_fields = OxmlElement("w:updateFields")
        settings.append(update_fields)
    update_fields.set(qn("w:val"), "true")


def keep_heading_relationships(paragraphs) -> None:
    for paragraph in paragraphs:
        if paragraph.style.name in {"Heading 1", "Heading 2", "Heading 3", "Heading 4", "Caption"}:
            paragraph.paragraph_format.keep_with_next = True


def update_document_versions(document, cover_version_cell, identity_version_cell) -> None:
    cover_version_cell.paragraphs[0].text = "V1.1"
    identity_version_cell.paragraphs[0].text = "V1.1"
    document.core_properties.version = "1.1"
