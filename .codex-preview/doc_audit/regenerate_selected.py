from pathlib import Path

import modify_docs


source_dir = modify_docs.Path(r"F:\@上位机标书\GB438C\补充文档\最终版文档")
output_dir = Path("修改版")
sources = modify_docs.find_sources(source_dir)

for key in ("Q", "R"):
    spec = next(item for item in modify_docs.SPECS if item.key == key)
    modify_docs.process_document(
        sources[key],
        output_dir / spec.output_name,
        spec,
    )
    print(f"regenerated: {spec.output_name}")

print(f"regenerated: {modify_docs.create_summary(output_dir).name}")
