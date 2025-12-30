import math

# Dartboard numbers order (top, clockwise)
NUMBERS = [
    20, 5, 12, 9, 14, 11, 8, 16, 7, 19,
    3, 17, 2, 15, 10, 6, 13, 4, 18, 1
]

SEGMENTS = 20

COLORS_SINGLE = ["black", "white"]
COLORS_DOUBLE = ["red", "green"]
COLORS_TRIPLE = ["red", "green"]
COLOR_BULL = "green"
COLOR_DOUBLE_BULL = "red"

# Radial proportions (relative to R)
PROPORTIONS = {
    "double_outer": 1.00,
    "double_inner": 0.94,
    "single_outer": 0.94,
    "single_inner": 0.64,
    "triple_outer": 0.64,
    "triple_inner": 0.58,
    "bull_outer": 0.10,
    "bull_inner": 0.05,
    "number_radius": 1.06,
    "number_ring_outer": 1.12,
    "number_ring_inner": 0.98,
}

# Backend zone codes
ZONE_CODES = {
    "single_int": 0,
    "single_ext": 1,
    "double": 2,
    "triple": 3,
    "bull": 1,
    "double_bull": 2,
}


class Dartboard:
    def __init__(self, canvas):
        self.canvas = canvas
        self.center = (0, 0, 0)
        self.previous_tags = []
        self.original_fills = {}

    def reset_highlight(self):
        for tag in self.previous_tags:
            fill = self.original_fills.get(tag)
            if fill:
                self.canvas.itemconfig(tag, fill=fill, outline="black", width=1)
        self.previous_tags.clear()

    def highlight(self, tag):
        self.reset_highlight()
        self.canvas.itemconfig(tag, fill="yellow", outline="black", width=2)
        self.previous_tags.append(tag)

    def draw(self, width, height):
        self.canvas.delete("all")
        self.original_fills.clear()

        size = min(width, height)
        R = size / 2 * 0.85
        cx, cy = width / 2, height / 2
        self.center = (cx, cy, R)

        angle_step = 360 / SEGMENTS

        def draw_arc(r1, r2, start, extent, color, tag):
            arc = self.canvas.create_arc(
                cx - r2, cy - r2,
                cx + r2, cy + r2,
                start=start,
                extent=extent,
                style="pieslice",
                fill=color,
                outline="black",
                tags=tag,
            )
            self.original_fills[tag] = color

            if r1 > 0:
                self.canvas.create_arc(
                    cx - r1, cy - r1,
                    cx + r1, cy + r1,
                    start=start,
                    extent=extent,
                    style="pieslice",
                    fill="white",
                    outline="white",
                )

        # Number ring
        self.canvas.create_oval(
            cx - R * PROPORTIONS["number_ring_outer"],
            cy - R * PROPORTIONS["number_ring_outer"],
            cx + R * PROPORTIONS["number_ring_outer"],
            cy + R * PROPORTIONS["number_ring_outer"],
            fill="black",
            outline="black",
        )

        self.canvas.create_oval(
            cx - R * PROPORTIONS["number_ring_inner"],
            cy - R * PROPORTIONS["number_ring_inner"],
            cx + R * PROPORTIONS["number_ring_inner"],
            cy + R * PROPORTIONS["number_ring_inner"],
            fill="white",
            outline="white",
        )

        # Segments
        for i in range(SEGMENTS):
            start_angle = i * angle_step + 81
            idx = i % 2

            draw_arc(
                R * PROPORTIONS["double_inner"],
                R * PROPORTIONS["double_outer"],
                start_angle,
                angle_step,
                COLORS_DOUBLE[idx],
                f"seg_{i}_double",
            )

            draw_arc(
                R * PROPORTIONS["single_inner"],
                R * PROPORTIONS["single_outer"],
                start_angle,
                angle_step,
                COLORS_SINGLE[idx],
                f"seg_{i}_outer_single",
            )

            draw_arc(
                R * PROPORTIONS["triple_inner"],
                R * PROPORTIONS["triple_outer"],
                start_angle,
                angle_step,
                COLORS_TRIPLE[idx],
                f"seg_{i}_triple",
            )

            draw_arc(
                0,
                R * PROPORTIONS["triple_inner"],
                start_angle,
                angle_step,
                COLORS_SINGLE[idx],
                f"seg_{i}_inner_single",
            )

            # Numbers
            angle_rad = math.radians(-(start_angle + angle_step / 2))
            rx = R * PROPORTIONS["number_radius"]
            self.canvas.create_text(
                cx + rx * math.cos(angle_rad),
                cy + rx * math.sin(angle_rad),
                text=str(NUMBERS[i]),
                fill="white",
                font=("Helvetica", int(R * 0.04), "bold"),
            )

        # Bulls
        self.canvas.create_oval(
            cx - R * PROPORTIONS["bull_outer"],
            cy - R * PROPORTIONS["bull_outer"],
            cx + R * PROPORTIONS["bull_outer"],
            cy + R * PROPORTIONS["bull_outer"],
            fill=COLOR_BULL,
            outline="black",
            tags="bull",
        )
        self.original_fills["bull"] = COLOR_BULL

        self.canvas.create_oval(
            cx - R * PROPORTIONS["bull_inner"],
            cy - R * PROPORTIONS["bull_inner"],
            cx + R * PROPORTIONS["bull_inner"],
            cy + R * PROPORTIONS["bull_inner"],
            fill=COLOR_DOUBLE_BULL,
            outline="black",
            tags="double_bull",
        )
        self.original_fills["double_bull"] = COLOR_DOUBLE_BULL

    def detect_hit(self, x, y):
        cx, cy, R = self.center
        dx, dy = x - cx, y - cy
        dist = math.hypot(dx, dy)

        # Bulls
        if dist <= R * PROPORTIONS["bull_inner"]:
            return 0, ZONE_CODES["double_bull"], "double_bull"

        if dist <= R * PROPORTIONS["bull_outer"]:
            return 0, ZONE_CODES["bull"], "bull"

        # Angle
        angle = (math.degrees(math.atan2(-dy, dx)) + 360) % 360
        angle = (angle - 81 + 360) % 360
        segment = int(angle // 18)
        number = NUMBERS[segment]

        # Regular zones
        if dist <= R * PROPORTIONS["triple_inner"]:
            return number, ZONE_CODES["single_int"], f"seg_{segment}_inner_single"

        if dist <= R * PROPORTIONS["triple_outer"]:
            return number, ZONE_CODES["triple"], f"seg_{segment}_triple"

        if dist <= R * PROPORTIONS["single_outer"]:
            return number, ZONE_CODES["single_ext"], f"seg_{segment}_outer_single"

        if dist <= R * PROPORTIONS["double_outer"]:
            return number, ZONE_CODES["double"], f"seg_{segment}_double"

        return None, None, None
