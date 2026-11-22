#ifndef VIDEODISPLAYGEOMETRY_H
#define VIDEODISPLAYGEOMETRY_H

#include <algorithm>
#include <cmath>

// Shared CPU-side geometry and GLSL for arbitrary-angle video display rotation.
// The display keeps the source aspect ratio and crops to the largest centered
// axis-aligned rectangle that contains no black corners after rotation.
namespace VideoDisplayGeometry {

struct Geometry {
    double sourceWidth = 0.0;
    double sourceHeight = 0.0;
    double cropWidth = 0.0;
    double cropHeight = 0.0;
    double centerX = 0.0;
    double centerY = 0.0;
    double rotationCos = 1.0;
    double rotationSin = 0.0;

    bool isEmpty() const
    {
        return sourceWidth <= 0.0 || sourceHeight <= 0.0 ||
               cropWidth <= 0.0 || cropHeight <= 0.0;
    }
};

inline Geometry calculate(double sourceWidth,
                          double sourceHeight,
                          double rotationDegrees)
{
    Geometry result;
    if (sourceWidth <= 0.0 || sourceHeight <= 0.0)
        return result;

    const double radians = rotationDegrees * 3.14159265358979323846 / 180.0;
    const double rotationCos = std::cos(radians);
    const double rotationSin = std::sin(radians);
    const double cosAbs = std::abs(rotationCos);
    const double sinAbs = std::abs(rotationSin);
    const double aspect = sourceWidth / sourceHeight;

    // Inverse-map an upright output rectangle into the rotated source. Both
    // half-width and half-height constraints must stay inside the source.
    const double widthByHeight =
            sourceHeight / (sinAbs + cosAbs / aspect);
    const double widthByWidth =
            sourceWidth / (cosAbs + sinAbs / aspect);

    result.sourceWidth = sourceWidth;
    result.sourceHeight = sourceHeight;
    result.cropWidth = std::min(widthByWidth, widthByHeight);
    result.cropHeight = result.cropWidth / aspect;
    result.centerX = sourceWidth * 0.5;
    result.centerY = sourceHeight * 0.5;
    result.rotationCos = rotationCos;
    result.rotationSin = rotationSin;
    return result;
}

inline const char *fragmentShader()
{
    return
        "varying vec2 vTexCoord;\n"
        "uniform sampler2D uTexture;\n"
        "uniform vec2 uSourceSize;\n"
        "uniform vec2 uCropSize;\n"
        "uniform vec2 uCropCenter;\n"
        "uniform vec2 uRotationCosSin;\n"
        "void main() {\n"
        "    vec2 cropPixel = (vTexCoord - vec2(0.5)) * uCropSize;\n"
        "    vec2 sourcePixel = uCropCenter +\n"
        "        vec2(uRotationCosSin.x * cropPixel.x - uRotationCosSin.y * cropPixel.y,\n"
        "             uRotationCosSin.y * cropPixel.x + uRotationCosSin.x * cropPixel.y);\n"
        "    vec2 uv = sourcePixel / uSourceSize;\n"
        "    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {\n"
        "        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "    } else {\n"
        "        gl_FragColor = texture2D(uTexture, uv);\n"
        "    }\n"
        "}\n";
}

} // namespace VideoDisplayGeometry

#endif // VIDEODISPLAYGEOMETRY_H
