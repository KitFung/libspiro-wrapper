extern "C" {
#include "libspiro/bezctx.h" /* bezctx structure */
#include "libspiro/spiro-config.h"
#include "libspiro/spiroentrypoints.h" /* call spiro through here */
}
#include "spiro_wrapper.h"

namespace spiro {
struct BezctxCrazy {  // Don't try to inherit it
  bezctx base;        // This must be the first element of the custom class
  double lastX, lastY;
  std::vector<KnotResult> *results;
};

static void CrazyMoveto(bezctx *bc, double x, double y, int is_open) {
  BezctxCrazy *g = reinterpret_cast<BezctxCrazy *>(bc);

  KnotResult result;
  result.type = KnotResult::Type::MOVE;
  result.points.emplace_back(x, y);
  result.fromX = g->lastX;
  result.fromY = g->lastY;
  g->results->push_back(result);

  g->lastX = x;
  g->lastY = y;
}

static void CrazyLineto(bezctx *bc, double x, double y) {
  BezctxCrazy *g = reinterpret_cast<BezctxCrazy *>(bc);

  KnotResult result;
  result.type = KnotResult::Type::LINE;
  result.points.emplace_back(x, y);
  result.fromX = g->lastX;
  result.fromY = g->lastY;
  g->results->push_back(result);

  g->lastX = x;
  g->lastY = y;
}

static void CrazyCurveto(bezctx *bc, double x1, double y1, double x2, double y2,
                         double x3, double y3) {
  BezctxCrazy *g = reinterpret_cast<BezctxCrazy *>(bc);

  KnotResult result;
  result.type = KnotResult::Type::CURVE;
  result.points.emplace_back(x1, y1);
  result.points.emplace_back(x2, y2);
  result.points.emplace_back(x3, y3);
  result.fromX = g->lastX;
  result.fromY = g->lastY;
  g->results->push_back(result);

  g->lastX = x3;
  g->lastY = y3;
}

static void CrazyMarkKnot(bezctx *bc, int knot_idx) {}

std::shared_ptr<BezctxCrazy> NewBezctxCrazy() {
  auto result = std::shared_ptr<BezctxCrazy>(new BezctxCrazy());
  result->base.moveto = CrazyMoveto;
  result->base.lineto = CrazyLineto;
  result->base.curveto = CrazyCurveto;
  result->base.mark_knot = CrazyMarkKnot;
  return result;
}

SpiroWrapper::SpiroWrapper(const std::vector<SpiroCP> &src, bool is_closed) {
  n_ = static_cast<int>(src.size());
  src_.reserve(n_);
  for (const auto &s : src) {
    src_.emplace_back(s);
  }

  is_closed_ = is_closed;
  valid_ = false;
  computed_ = false;
}

SpiroWrapper::~SpiroWrapper() {}

bool SpiroWrapper::Compute() {
  if (computed_) {
    return valid_;
  }

  std::unique_ptr<spiro_cp[]> src(new spiro_cp[n_]);
  for (int i = 0; i < n_; ++i) {
    auto &s = (src.get())[i];
    s.x = src_[i].x;
    s.y = src_[i].y;
    s.ty = static_cast<char>(src_[i].ty);
  }

  auto bc = NewBezctxCrazy();
  bc->results = &results_;
  valid_ =
      SpiroCPsToBezier0(src.get(), n_, (is_closed_ ? 1 : 0), &(bc->base)) == 1;

  return valid_;
}

bool SpiroWrapper::Valid() const { return valid_; }

const std::vector<KnotResult> &SpiroWrapper::Results() const {
  return results_;
}

}  // namespace spiro
