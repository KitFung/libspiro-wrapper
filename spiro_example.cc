#include <iostream>
#include <memory>
#include <vector>

extern "C" {
#include "libspiro/bezctx.h" /* bezctx structure */
#include "libspiro/spiro-config.h"
#include "libspiro/spiroentrypoints.h" /* call spiro through here */
}

struct bezctx_custom {  // Don't try to inherit it
  bezctx base;  // This must be the first element of the custom class
  std::vector<std::pair<double, double>> vec;
  void add2vec(double x, double y) {
    vec.push_back(std::make_pair(x, y));
    printf("bezctx_custom: add2vec %g %g\n", x, y);
  }
};
typedef std::shared_ptr<bezctx_custom> BezctxCustomPtr;

void test_moveto(bezctx *bc, double x, double y, int is_open) {
  printf("test_moveto(%g,%g)_%d\n", x, y, is_open);
  bezctx_custom *g = reinterpret_cast<bezctx_custom *>(bc);
  g->add2vec(x, y);
}
void test_lineto(bezctx *bc, double x, double y) {
  printf("test_lineto(%g,%g)\n", x, y);
  bezctx_custom *g = reinterpret_cast<bezctx_custom *>(bc);
  g->add2vec(x, y);
}
void test_curveto(bezctx *bc, double x1, double y1, double x2, double y2,
                  double x3, double y3) {
  printf("test_curveto(%g,%g, %g,%g, %g,%g)\n", x1, y1, x2, y2, x3, y3);
  bezctx_custom *g = reinterpret_cast<bezctx_custom *>(bc);
  g->add2vec(x1, y1);
  g->add2vec(x2, y2);
  g->add2vec(x3, y3);
}
void test_mark_knot(bezctx *bc, int knot_idx) {
  printf("test_mark_knot()_%d\n", knot_idx);
}

BezctxCustomPtr new_bezctx_test() {
  auto result = std::shared_ptr<bezctx_custom>(new bezctx_custom());
  result->base.moveto = test_moveto;
  result->base.lineto = test_lineto;
  result->base.curveto = test_curveto;
  result->base.mark_knot = test_mark_knot;
  return result;
}

void exampleA() {
  spiro_cp spiro0[] = {
      {334, 117, 'v'}, {305, 176, 'v'}, {212, 142, 'c'}, {159, 171, 'c'},
      {224, 237, 'c'}, {347, 335, 'c'}, {202, 467, 'c'}, {81, 429, 'v'},
      {114, 368, 'v'}, {201, 402, 'c'}, {276, 369, 'c'}, {218, 308, 'c'},
      {91, 211, 'c'},  {124, 111, 'c'}, {229, 82, 'c'},  {0, 0, 'z'}};
  auto bc = new_bezctx_test();
  int success = SpiroCPsToBezier0(spiro0, 16, 1, &(bc->base));
  if (!success) {
    std::cout << "Failed in example" << std::endl;
  }
}

int main() {
  exampleA();
  return 0;
}
