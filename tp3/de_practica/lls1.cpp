
#include  <boost/numeric/ublas/io.hpp>
#include  <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include  <boost/numeric/bindings/lapack/gels.hpp>
#include  <boost/numeric/bindings/traits/ublas_vector2.hpp>


namespace ublas = boost::numeric::ublas;
namespace lapack= boost::numeric::bindings::lapack;


int main()
{
    ublas::matrix<float,ublas::row_major> A(3,3);
    ublas::vector<float> b(3);


    for(unsigned i=0;i < A.size1();i++)
        for(unsigned j =0;j < A.size2();j++)
        {
            A(i,j) = 1.0;
        }

    std::cout << A << std::endl;

    b(0) = 21; b(1) = 1; b(2) = 17;

   int x = lapack::gels('N', A, b, lapack::minimal_workspace());


    return 0;
}
