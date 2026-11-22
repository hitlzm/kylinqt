from __future__ import annotations

import sys
from pathlib import Path

import pypdfium2 as pdfium
from PIL import Image


def make_contact_sheet(images: list[Image.Image], output: Path, cols: int = 4) -> None:
    thumb_width = 320
    thumb_height = 452
    gutter = 16
    label_height = 28
    rows = (len(images) + cols - 1) // cols
    canvas = Image.new(
        "RGB",
        (
            cols * thumb_width + (cols + 1) * gutter,
            rows * (thumb_height + label_height) + (rows + 1) * gutter,
        ),
        "white",
    )
    for index, image in enumerate(images):
        row, col = divmod(index, cols)
        image.thumbnail((thumb_width, thumb_height), Image.Resampling.LANCZOS)
        x = gutter + col * (thumb_width + gutter)
        y = gutter + row * (thumb_height + label_height + gutter)
        canvas.paste(image, (x, y))
    canvas.save(output, quality=92)


def main() -> None:
    if len(sys.argv) not in {3, 4}:
        raise SystemExit(
            "usage: rasterize_pdfs.py <pdf_dir> <output_dir> [pdf_name]"
        )
    pdf_dir = Path(sys.argv[1])
    output_dir = Path(sys.argv[2])
    output_dir.mkdir(parents=True, exist_ok=True)
    summary = []
    pdf_name = sys.argv[3] if len(sys.argv) == 4 else "*"
    for pdf_path in sorted(pdf_dir.glob(pdf_name)):
        doc = pdfium.PdfDocument(pdf_path)
        pages = []
        doc_dir = output_dir / pdf_path.stem
        doc_dir.mkdir(parents=True, exist_ok=True)
        for index in range(len(doc)):
            page_image = doc[index].render(scale=1.6).to_pil().convert("RGB")
            page_path = doc_dir / f"page-{index + 1:03d}.png"
            page_image.save(page_path)
            pages.append(page_image)
        make_contact_sheet(pages, output_dir / f"{pdf_path.stem}-contact.png")
        summary.append((pdf_path.name, len(doc)))
        doc.close()
    for name, count in summary:
        print(f"{name}: {count} pages")


if __name__ == "__main__":
    main()
