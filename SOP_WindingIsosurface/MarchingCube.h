#include <openvdb/openvdb.h>
#include <vector>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Math.h>
#include <functional>
namespace Geometry
{
    typedef int64_t Int64;
    class MarchingCube
    {
    public:
        struct TriangleIndices
        {
            int value[3];
        };
        void Build(
            std::vector<UT_Vector3D>& seeds,
            double resolution,
            double isovalue,
            UT_Vector3D bound,
            UT_Vector3D fieldOffset
        );
        void Clear();
        std::vector<TriangleIndices>& GetIndices();
        std::vector<UT_Vector3D>& GetVertices();
        std::function<double(double, double, double)> implicit;
    private:
        void BuildInternal(
            std::vector<UT_Vector3D>& seeds,
            double resolution,
            double isovalue,
            UT_Vector3I bound,
            UT_Vector3D fieldOffset
        );
        bool TrySetCellActive(const UT_Vector3I& cell);
        bool CellIntersectSurface(const UT_Vector3I& cell, double resolution, double isovalue, const UT_Vector3D& fieldOffset);
        void BuildPolygonInCell(const UT_Vector3I& cell, double resolution, double isovalue, const UT_Vector3D& fieldOffset);
        int CalcCornerSignBitmap(const UT_Vector3I& cell, double resolution, double isovalue, const UT_Vector3D& fieldOffset);
        int GetVertexIndexOnEdge(const UT_Vector3I& s, const UT_Vector3I& e, double resolution, double isovalue, const UT_Vector3D& fieldOffset);
        void AddTriangles(int a, int b, int c, Int64 cellHash);
        void AppendTriangles(const UT_Vector3I& bound, const UT_Vector3I& offset);
        void InitBuildCache();
        UT_Vector3D RootFind(const UT_Vector3I& s, const UT_Vector3I& e, double resolution, double isovalue, const UT_Vector3D& fieldOffset);
    private:
        std::vector<TriangleIndices> _indices;
        std::vector<UT_Vector3D> _vertices;
        openvdb::Int32Grid _BorderVertices;

        openvdb::BoolGrid _VisitedCells;
        const static int kEdgeVerticesSectionNum = 64;
        std::unordered_map<Int64, int> _edgeVertexIndicesSections[kEdgeVerticesSectionNum];
        std::mutex _edgeVertexIndicesSectionMutex[kEdgeVerticesSectionNum];

        std::atomic_int _tempVertexNum;
        std::unordered_map<Int64, UT_Vector3D> _edgeVertexSections[kEdgeVerticesSectionNum];
        std::mutex _edgeVertexSectionMutex[kEdgeVerticesSectionNum];

        const static int kCellSectionNum = 64;
        std::vector<int> _triangleSections[kCellSectionNum];
        std::mutex _triangleSectionMutex[kCellSectionNum];
    };
}