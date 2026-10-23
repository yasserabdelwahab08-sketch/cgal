#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/Straight_skeleton_builder_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K ;

typedef K::Point_2                   Point ;
typedef CGAL::Polygon_2<K>           Polygon_2 ;
typedef CGAL::Straight_skeleton_2<K> Ss ;

typedef std::shared_ptr<Polygon_2> PolygonPtr ;
typedef std::shared_ptr<Ss> SsPtr ;

typedef std::vector<PolygonPtr> PolygonPtrVector ;

void test_enter_contour_weights_2_arg()
{
  typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
  typedef Kernel::FT                                           FT;
  typedef CGAL::Straight_skeleton_2<Kernel>                    Ss;
  typedef CGAL::Straight_skeleton_builder_2<Kernel, Ss>        Builder;

  Builder builder;
  std::vector<FT> weights = { 1.0, 1.0, 1.0 };

  // Calls the 2-argument overload containing the typo fix.
  // Before the fix, this fails to compile due to 'enter_contour_weightsg'.
  // After the fix, this compiles cleanly.
  builder.enter_contour_weights(weights.begin(), weights.end());
}

int main()
{
  std::ifstream in("data/A.poly");
  if (!in) return 1;
  int n;
  double x,y;
  Polygon_2 poly ;

  in >> n; // skip #polylines
  in >> n;
  for (int i=0; i<n; ++i)
  {
    in >> x >> y;
    poly.push_back( Point(x,y) ) ;
  }
  // we are only taking the outer contour (sufficient for this test)

  std::cout << poly.size() << "\n";

  SsPtr ss = CGAL::create_interior_straight_skeleton_2(poly);
  Ss ss_copy(*ss);

  double lOffset = 5 ;

  PolygonPtrVector offset_polygons = CGAL::create_offset_polygons_2<Polygon_2>(lOffset,ss_copy);

  assert(offset_polygons.size()==1);
  assert(offset_polygons.front()->size()>10);
  test_enter_contour_weights_2_arg();
  return 0;
}
