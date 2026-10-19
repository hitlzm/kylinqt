# -*- coding: utf-8 -*-
"""Plot outputs from exguide_rate_compare using the dependency-free PNG helper."""

import math
import os

import plot

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")


def read(name):
    return plot.read_csv(name)


def draw():
    ab_laser = read("rate_ab_laser_sine_025hz.csv")
    ab_image = read("rate_ab_image_sine_025hz.csv")
    ab_stop = read("rate_ab_laser_stop.csv")
    px30 = read("rate_pixel_pixel30_sine_025hz.csv")
    px50 = read("rate_pixel_pixel50_sine_025hz.csv")
    px_stop = read("rate_pixel_pixel30_stop.csv")

    canvas = plot.Canvas(1720, 1220)
    canvas.text(30, 24, "EX-GUIDE FILTER RATE/TRAJECTORY TEST (PRODUCTION SOURCES)", plot.DARK, 2)
    canvas.text(
        30,
        47,
        "ANGLE: LASER 100HZ / IMAGE 50HZ.  PIXEL: DETECTOR 30/50FPS.  GREEN=TRUTH  RED=ESTIMATE",
        plot.GRAY,
        1,
    )

    x0, y0 = 130, 145
    pw, ph = 700, 280
    dx, dy = 760, 355

    plot.draw_panel(
        canvas, x0, y0, pw, ph,
        "A  ALPHA-BETA, LASER 100HZ, 0.25HZ +-6DEG",
        [ab_laser["t"], ab_laser["t"]],
        [ab_laser["truth"], ab_laser["estimate"]],
        [plot.GREEN, plot.RED],
        (4, 20), (-8, 8), "TIME (S)", "DEG",
        subtitle="FAST INPUT DOES NOT REMOVE THE CONSTANT-VELOCITY EXTRAPOLATION ERROR",
    )

    plot.draw_panel(
        canvas, x0 + dx, y0, pw, ph,
        "B  ALPHA-BETA, IMAGE 50HZ, 0.25HZ +-6DEG",
        [ab_image["t"], ab_image["t"]],
        [ab_image["truth"], ab_image["estimate"]],
        [plot.GREEN, plot.RED],
        (4, 20), (-8, 8), "TIME (S)", "DEG",
        subtitle="50HZ HAS MORE NOISE/JITTER, BUT THE TRACK IS BOUNDED",
    )

    plot.draw_panel(
        canvas, x0, y0 + dy, pw, ph,
        "C  ALPHA-BETA STOP: 2DEG/S RAMP THEN HOLD 10DEG",
        [ab_stop["t"], ab_stop["t"]],
        [ab_stop["truth"], ab_stop["estimate"]],
        [plot.GREEN, plot.RED],
        (0, 15), (0, 12), "TIME (S)", "DEG",
        subtitle="STOPS AT 5S; OUTPUT CONVERGES, NO NUMERICAL DIVERGENCE",
    )

    plot.draw_panel(
        canvas, x0 + dx, y0 + dy, pw, ph,
        "D  PIXEL KALMAN, 30FPS, 0.25HZ +-120PX",
        [px30["t"], px30["t"]],
        [px30["truth"], px30["estimate"]],
        [plot.GREEN, plot.RED],
        (4, 20), (480, 800), "TIME (S)", "PX",
        subtitle="Q=0.01: SMOOTH BUT LARGE MODEL LAG ON ACCELERATION/REVERSAL",
    )

    plot.draw_panel(
        canvas, x0, y0 + 2 * dy, pw, ph,
        "E  PIXEL KALMAN, 50FPS, 0.25HZ +-120PX",
        [px50["t"], px50["t"]],
        [px50["truth"], px50["estimate"]],
        [plot.GREEN, plot.RED],
        (4, 20), (480, 800), "TIME (S)", "PX",
        subtitle="HIGHER FRAME RATE DOES NOT FIX A TOO-SMALL Q ON A TURN",
    )

    plot.draw_panel(
        canvas, x0 + dx, y0 + 2 * dy, pw, ph,
        "F  PIXEL KALMAN STOP: 33PX/S RAMP THEN HOLD 465PX",
        [px_stop["t"], px_stop["t"]],
        [px_stop["truth"], px_stop["estimate"]],
        [plot.GREEN, plot.RED],
        (0, 20), (420, 560), "TIME (S)", "PX",
        subtitle="DEFAULT Q GIVES LARGE OVERSHOOT/TAIL AFTER THE TARGET STOPS",
    )

    canvas.text(30, 1180, "FULL NUMERIC RESULT: OUT/RATE_SUMMARY.CSV   TIME SERIES: OUT/RATE_*.CSV", plot.GRAY, 1)
    path = os.path.join(OUT, "rate_compare.png")
    canvas.save(path)
    print("written:", path)


if __name__ == "__main__":
    draw()
