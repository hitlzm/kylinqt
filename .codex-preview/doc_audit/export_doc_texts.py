from __future__ import annotations

import json
import sys
from pathlib import Path


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: export_doc_texts.py <audit_json> <output_dir>")
    records = json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
    output_dir = Path(sys.argv[2])
    output_dir.mkdir(parents=True, exist_ok=True)
    for index, record in enumerate(records, 1):
        lines = [
            f"FILE: {record['name']}",
            f"CORE: {json.dumps(record['core_properties'], ensure_ascii=False)}",
            f"CUSTOM: {json.dumps(record['custom_properties'], ensure_ascii=False)}",
            "",
        ]
        for item in record["text_items"]:
            lines.append(
                f"[{item['location']}][{item['style']}][{item['align']}] "
                f"{item['text']}"
            )
        target = output_dir / f"{index:02d}.txt"
        target.write_text("\n".join(lines), encoding="utf-8")
    print(f"exported {len(records)} text files -> {output_dir}")


if __name__ == "__main__":
    main()
