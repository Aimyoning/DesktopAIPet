"""生成机器人的 .ico 图标（含 16x16 到 256x256 多尺寸）"""
from PIL import Image, ImageDraw

def draw_robot_icon(size):
    """绘制像素风机器人图标"""
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    s = size

    body = (0, 212, 255)       # 青色 #00d4ff
    body_dark = (0, 160, 200)
    white = (255, 255, 255)
    antenna_gold = (255, 200, 50)

    # 天线杆
    cx = s // 2
    ay = int(s * 0.08)
    draw.line([(cx, ay), (cx, int(s * 0.18))], fill=body, width=max(2, size // 60))

    # 天线球
    r = max(3, size // 20)
    draw.ellipse([cx - r, ay - r, cx + r, ay + r], fill=antenna_gold)

    # 头部
    hx, hy, hw, hh = cx, int(s * 0.28), int(s * 0.32), int(s * 0.26)
    draw.rounded_rectangle(
        [hx - hw//2, hy - hh//2, hx + hw//2, hy + hh//2],
        radius=size // 8, fill=body, outline=body_dark, width=max(1, size // 80))

    # 眼睛
    esp = max(5, size // 12)
    ew, eh = max(4, size // 18), max(5, size // 18)
    eye_y = hy - eh // 2
    draw.ellipse([hx - esp - ew//2, eye_y - eh//2, hx - esp + ew//2, eye_y + eh//2], fill=white)
    draw.ellipse([hx + esp - ew//2, eye_y - eh//2, hx + esp + ew//2, eye_y + eh//2], fill=white)

    # 瞳孔
    pr = max(1, size // 40)
    draw.ellipse([hx - esp - pr, eye_y - pr, hx - esp + pr, eye_y + pr], fill=(20, 20, 35))
    draw.ellipse([hx + esp - pr, eye_y - pr, hx + esp + pr, eye_y + pr], fill=(20, 20, 35))

    # 身体
    by = int(s * 0.54)
    bw, bh = int(s * 0.34), int(s * 0.30)
    draw.rounded_rectangle(
        [cx - bw//2, by - bh//2, cx + bw//2, by + bh//2],
        radius=size // 6, fill=body, outline=body_dark, width=max(1, size // 80))

    # 腿部
    lw, lh = max(3, size // 12), max(6, size // 8)
    ly = int(s * 0.75)
    draw.rounded_rectangle(
        [cx - lw - 2, ly, cx - 2, ly + lh], radius=size // 15, fill=body_dark)
    draw.rounded_rectangle(
        [cx + 2, ly, cx + lw + 2, ly + lh], radius=size // 15, fill=body_dark)

    return img


sizes = [16, 32, 48, 64, 128, 256]
images = []
for sz in sizes:
    images.append(draw_robot_icon(sz))

# 以 256x256 为主图，保存多尺寸 .ico
images[-1].save("robot.ico", format="ICO", sizes=[(s, s) for s in sizes],
                append_images=images[:-1])
print(f"robot.ico 已生成，含尺寸: {sizes}")
