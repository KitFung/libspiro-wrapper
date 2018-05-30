#ifndef SPIRO_WRAPPER_H_
#define SPIRO_WRAPPER_H_

#include <iostream>
#include <memory>
#include <vector>

namespace spiro {

enum class SpiroCPType : char {
  CORNER = 'v',
  G4 = 'o',
  G2 = 'c',
  LEFT = '[',
  RIGHT = ']',
  END = 'z',
  ANCHOR = 'a',
  HANDLE = 'h'
};

struct SpiroCP {
  double x;
  double y;
  SpiroCPType ty;
};

struct KnotResult {
  enum Type { MOVE, LINE, CURVE };
  Type type;

  double fromX, fromY;
  std::vector<std::pair<double, double>> points;

  void Println() {
    switch (type) {
      case Type::MOVE:
        std::cout << "Move: from (" << fromX << ", " << fromY << ") to ("
                  << points[0].first << ", " << points[0].second << ")"
                  << std::endl;
        break;
      case Type::LINE:
        std::cout << "Line: from (" << fromX << ", " << fromY << ") to ("
                  << points[0].first << ", " << points[0].second << ")"
                  << std::endl;
        break;
      case Type::CURVE:
        std::cout << "Curve: P0 (" << fromX << ", " << fromY << "),  "
                  << "P1 (" << points[0].first << ", " << points[0].second
                  << "),  "
                  << "P2 (" << points[1].first << ", " << points[1].second
                  << "),  "
                  << "P3 (" << points[2].first << ", " << points[2].second
                  << "),  " << std::endl;
        break;
      default:
        break;
    }
  }

  bool PointAtCubicBezier(double t, double *x, double *y) const {
    if (t < 0 || t > 1 || type != Type::CURVE || points.size() != 4) {
      return false;
    }
    const double co0 = (1 - t) * (1 - t) * (1 - t);
    const double co1 = (1 - t) * (1 - t) * t * 3;
    const double co2 = (1 - t) * t * t * 3;
    const double co3 = t * t * t;
    *x = fromX.first * co0 + points[0].first * co1 + points[1].first * co2 +
         points[2].first * co3;
    *y = fromY.second * co0 + points[0].second * co1 +
         points[1].second * co2 + points[2].second * co3;
    return true;
  }
};

class SpiroWrapper {
 public:
  SpiroWrapper(const std::vector<SpiroCP> &src, bool is_closed = false);
  ~SpiroWrapper();

  bool Compute();

  bool Valid() const;
  const std::vector<KnotResult> &Results() const;

 private:
  // Disable Implict Construct, Copy, Move
  SpiroWrapper();
  SpiroWrapper(const SpiroWrapper &);
  SpiroWrapper &operator=(const SpiroWrapper &);

  int n_;
  bool is_closed_;
  bool valid_, computed_;
  std::vector<SpiroCP> src_;
  std::vector<KnotResult> results_;
};

}  // namespace spiro

#endif
