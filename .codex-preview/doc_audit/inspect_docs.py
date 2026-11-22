from __future__ import annotations

import json
import re
import sys
import zipfile
from collections import Counter
from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml.ns import qn


def paragraph_text(paragraph) -> str:
    return paragraph.text.replace("\t", " ").strip()


def alignment_name(value) -> str:
    return {
        None: "inherit",
        WD_ALIGN_PARAGRAPH.LEFT: "left",
        WD_ALIGN_PARAGRAPH.CENTER: "center",
        WD_ALIGN_PARAGRAPH.RIGHT: "right",
        WD_ALIGN_PARAGRAPH.JUSTIFY: "justify",
        WD_ALIGN_PARAGRAPH.DISTRIBUTE: "distribute",
    }.get(value, str(value))


def run_font(run) -> str:
    if run.font.name:
        return run.font.name
    rfonts = run._element.rPr.rFonts if run._element.rPr is not None else None
    if rfonts is not None:
        return (
            rfonts.get(qn("w:eastAsia"))
            or rfonts.get(qn("w:ascii"))
            or rfonts.get(qn("w:hAnsi"))
            or ""
        )
    return ""


def compact_xml_text(element) -> str:
    if element is None:
        return ""
    texts = element.xpath(".//w:t")
    return " | ".join(
        node.text.strip() for node in texts if node.text and node.text.strip()
    )


def inspect_doc(path: Path) -> dict:
    doc = Document(path)
    props = doc.core_properties
    text_items: list[dict] = []
    paragraph_aligns: Counter[str] = Counter()
    paragraph_styles: Counter[str] = Counter()
    body_fonts: Counter[str] = Counter()
    body_sizes: Counter[str] = Counter()
    headings: list[dict] = []

    def add_item(location: str, source, paragraph=None) -> None:
        text = source.text if paragraph is not None else str(source)
        text = text.replace("\t", " ").strip()
        if not text:
            return
        style = paragraph.style.name if paragraph is not None else ""
        align = alignment_name(paragraph.alignment) if paragraph is not None else ""
        text_items.append(
            {
                "location": location,
                "style": style,
                "align": align,
                "text": text,
            }
        )
        if location == "body":
            paragraph_aligns[align] += 1
            paragraph_styles[style] += 1
            if paragraph is not None:
                for run in paragraph.runs:
                    if not run.text.strip():
                        continue
                    font = run_font(run)
                    if font:
                        body_fonts[font] += 1
                    if run.font.size is not None:
                        body_sizes[str(run.font.size.pt) + "pt"] += 1
        if style.lower().startswith("heading") or style.lower() in {"title", "subtitle"}:
            headings.append(
                {"style": style, "align": align, "text": text}
            )

    for paragraph in doc.paragraphs:
        add_item("body", paragraph, paragraph)

    for table_index, table in enumerate(doc.tables, 1):
        for row_index, row in enumerate(table.rows, 1):
            for cell_index, cell in enumerate(row.cells, 1):
                for paragraph in cell.paragraphs:
                    add_item(
                        f"table{table_index}:{row_index}:{cell_index}",
                        paragraph,
                        paragraph,
                    )

    section_data = []
    for section_index, section in enumerate(doc.sections, 1):
        section_data.append(
            {
                "index": section_index,
                "page_width_in": round(section.page_width.inches, 3),
                "page_height_in": round(section.page_height.inches, 3),
                "top_margin_in": round(section.top_margin.inches, 3),
                "bottom_margin_in": round(section.bottom_margin.inches, 3),
                "left_margin_in": round(section.left_margin.inches, 3),
                "right_margin_in": round(section.right_margin.inches, 3),
                "header_distance_in": round(section.header_distance.inches, 3),
                "footer_distance_in": round(section.footer_distance.inches, 3),
                "different_first_page": section.different_first_page_header_footer,
                "header_linked_to_previous": section.header.is_linked_to_previous,
                "footer_linked_to_previous": section.footer.is_linked_to_previous,
                "header_text": compact_xml_text(section.header._element),
                "footer_text": compact_xml_text(section.footer._element),
            }
        )

    all_text = "\n".join(item["text"] for item in text_items)
    version_patterns = [
        r"(?:版本|Version|VERSION)\s*(?:号|No\.?)?\s*[:：]?\s*([^\s，,；;。]{1,32})",
        r"\bV\s*\d+(?:\.\d+){1,3}\b",
    ]
    versions = []
    for pattern in version_patterns:
        versions.extend(re.findall(pattern, all_text, flags=re.IGNORECASE))

    date_patterns = [
        r"\b20\d{2}\s*[年./-]\s*\d{1,2}\s*[月./-]\s*\d{1,2}\s*日?",
        r"\b20\d{2}\s*年\s*\d{1,2}\s*月\b",
    ]
    dates = []
    for pattern in date_patterns:
        dates.extend(re.findall(pattern, all_text))

    ccd_lines = [
        item for item in text_items if re.search(r"CCD|相机", item["text"], re.I)
    ]

    custom_props = {}
    with zipfile.ZipFile(path) as archive:
        if "docProps/custom.xml" in archive.namelist():
            custom_xml = archive.read("docProps/custom.xml").decode(
                "utf-8", errors="ignore"
            )
            for name, value in re.findall(
                r'name="([^"]+)".*?<vt:lpwstr>(.*?)</vt:lpwstr>',
                custom_xml,
                flags=re.DOTALL,
            ):
                custom_props[name] = value

    return {
        "file": str(path),
        "name": path.name,
        "size_bytes": path.stat().st_size,
        "file_modified": path.stat().st_mtime,
        "core_properties": {
            "title": props.title,
            "subject": props.subject,
            "author": props.author,
            "last_modified_by": props.last_modified_by,
            "created": props.created.isoformat() if props.created else "",
            "modified": props.modified.isoformat() if props.modified else "",
            "revision": props.revision,
            "version": props.version,
            "comments": props.comments,
            "keywords": props.keywords,
        },
        "custom_properties": custom_props,
        "detected_versions": list(dict.fromkeys(versions)),
        "detected_dates": list(dict.fromkeys(dates)),
        "sections": section_data,
        "body_alignments": dict(paragraph_aligns),
        "body_styles": dict(paragraph_styles),
        "body_fonts": dict(body_fonts),
        "body_sizes": dict(body_sizes),
        "headings": headings,
        "ccd_lines": ccd_lines,
        "text_items": text_items,
    }


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: inspect_docs.py <source_dir> <output_json>")
    source_dir = Path(sys.argv[1])
    output_json = Path(sys.argv[2])
    output_json.parent.mkdir(parents=True, exist_ok=True)
    records = []
    for path in sorted(source_dir.glob("*.docx")):
        records.append(inspect_doc(path))
    output_json.write_text(
        json.dumps(records, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    print(f"inspected {len(records)} documents -> {output_json}")


if __name__ == "__main__":
    main()
