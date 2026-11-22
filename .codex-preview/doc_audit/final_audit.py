from __future__ import annotations

import re
from pathlib import Path

from docx import Document
from docx.oxml.ns import qn

import modify_docs


ROOT = Path("修改版")
OLD_TERM = re.compile(r"CCD|相机|电荷耦合", re.IGNORECASE)


def paragraph_texts(doc: Document):
    for paragraph in doc.paragraphs:
        yield paragraph.text
    for table in doc.tables:
        for row in table.rows:
            for cell in row.cells:
                for paragraph in cell.paragraphs:
                    yield paragraph.text
    seen = set()
    for section in doc.sections:
        for container in (
            section.header,
            section.footer,
            section.first_page_header,
            section.first_page_footer,
            section.even_page_header,
            section.even_page_footer,
        ):
            element_id = id(container._element)
            if element_id in seen:
                continue
            seen.add(element_id)
            for node in container._element.iter(qn("w:t")):
                yield node.text or ""


registered = {spec.output_name: spec for spec in modify_docs.SPECS}
paths = sorted(ROOT.glob("*.docx"))
errors: list[str] = []

for path in paths:
    doc = Document(path)
    spec = registered.get(path.name)
    expected_version = spec.target_version if spec else "V1.1"
    if doc.core_properties.version != expected_version:
        errors.append(
            f"{path.name}: metadata version {doc.core_properties.version!r} "
            f"!= {expected_version!r}"
        )

    for index, section in enumerate(doc.sections, 1):
        dimensions = {
            "page_width": round(section.page_width.inches, 3),
            "page_height": round(section.page_height.inches, 3),
            "top": round(section.top_margin.inches, 3),
            "bottom": round(section.bottom_margin.inches, 3),
            "left": round(section.left_margin.inches, 3),
            "right": round(section.right_margin.inches, 3),
            "header": round(section.header_distance.inches, 3),
            "footer": round(section.footer_distance.inches, 3),
        }
        expected = {
            "page_width": 8.268,
            "page_height": 11.693,
            "top": 1.0,
            "bottom": 1.0,
            "left": 1.252,
            "right": 1.252,
            "header": 0.59,
            "footer": 0.63,
        }
        if dimensions != expected:
            errors.append(f"{path.name}: section {index} dimensions {dimensions}")

    texts = list(paragraph_texts(doc))
    old_lines = [text for text in texts if OLD_TERM.search(text)]
    if old_lines:
        errors.append(f"{path.name}: old device term remains: {old_lines[:2]}")
    repeated_dates = [text for text in texts if re.search(r"日(?:15日)+", text)]
    if repeated_dates:
        errors.append(f"{path.name}: repeated date fragment remains")

    if spec:
        expected_date = spec.revision_date
        if expected_date not in "\n".join(texts):
            errors.append(f"{path.name}: expected date {expected_date} not found")

if len(paths) != 21:
    errors.append(f"expected 21 DOCX files, found {len(paths)}")

print(f"files={len(paths)}")
print(f"errors={len(errors)}")
for error in errors:
    print(error)

raise SystemExit(1 if errors else 0)
