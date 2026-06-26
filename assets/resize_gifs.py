"""缩放 leisure.gif 和 speak.gif 适配 200x280 显示区域"""
from PIL import Image, ImageSequence

TARGET_W = 200   # 目标宽度
TARGET_H = 280   # 目标高度


def resize_gif(src_path, dst_path):
    """缩放 GIF 的每一帧，等比缩放适配目标区域"""
    src = Image.open(src_path)
    orig_w, orig_h = src.size

    # 计算缩放比例（等比缩放，宽或高适配）
    scale = min(TARGET_W / orig_w, TARGET_H / orig_h)
    new_w = int(orig_w * scale)
    new_h = int(orig_h * scale)

    frames = []
    for frame in ImageSequence.Iterator(src):
        # 用 LANCZOS 高质量缩放
        resized = frame.copy().resize((new_w, new_h), Image.LANCZOS)
        frames.append(resized)

    # 保存，保留原始帧间隔
    durations = []
    for i in range(src.n_frames):
        src.seek(i)
        durations.append(src.info.get('duration', 100))

    frames[0].save(
        dst_path, save_all=True, append_images=frames[1:],
        duration=durations, loop=0, disposal=2
    )
    print(f"{src_path.name} {orig_w}x{orig_h} → {dst_path.name} {new_w}x{new_h}")


from pathlib import Path
base = Path("D:/Mycode/Qt_code/DesktopAIPet/assets")

# leisure → idle
resize_gif(base / "leisure.gif", base / "idle.gif")

# speak → talk
resize_gif(base / "speak.gif", base / "talk.gif")

print("✅ 完成")
