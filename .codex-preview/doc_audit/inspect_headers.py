from __future__ import annotations

import sys
from pathlib import Path

from docx import Document
from docx.oxml.ns import qn


def describe_paragraph(paragraph) -> str:
    parts = []
    for run in paragraph.runs:
        text = run.text.replace("\n", "\\n")
        if not text:
            continue
        rfonts = run._element.rPr.rFonts if run._element.rPr is not None else None
        font = ""
        if rfonts is not None:
            font = (
                rfonts.get(qn("w:eastAsia"))
                or rfonts.get(qn("w:ascii"))
                or rfonts.get(qn("w:hAnsi"))
                or ""
            )
        size = f"{run.font.size.pt}pt" if run.font.size else ""
        parts.append(f"{text!r}[{font},{size}]")
    return (
        f"text={paragraph.text!r}; align={paragraph.alignment}; "
        f"left={paragraph.paragraph_format.left_indent}; "
        f"right={paragraph.paragraph_format.right_indent}; "
        f"first={paragraph.paragraph_format.first_line_indent}; runs={' '.join(parts)}"
    )


def main() -> None:
    for name in sys.argv[1:]:
        path = Path(name)
        doc = Document(path)
        print(f"FILE: {path.name}")
        for index, section in enumerate(doc.sections, 1):
            print(
                f"  SECTION {index}: linked={section.header.is_linked_to_previous}; "
                f"distance={section.header_distance.inches}"
            )
            for p_idx, paragraph in enumerate(section.header.paragraphs, 1):
                print(f"    P{p_idx}: {describe_paragraph(paragraph)}")
            for t_idx, table in enumerate(section.header.tables, 1):
                print(f"    TABLE{t_idx}: {len(table.rows)}x{len(table.columns)}")
                for r_idx, row in enumerate(table.rows, 1):
                    for c_idx, cell in enumerate(row.cells, 1):
                        for p in cell.paragraphs:
                            if p.text.strip():
                                print(
                                    f"      R{r_idx}C{c_idx}: {describe_paragraph(p)}"
                                )
        print()


if __name__ == "__main__":
    main()
