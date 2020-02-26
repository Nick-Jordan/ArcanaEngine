#ifndef MARCHING_CUBES_H_
#define MARCHING_CUBES_H_

#include "Types.h"

#include <unordered_map>
#include <vector>

namespace Arcana
{
    typedef float VertexComponentsType;
    typedef int32 QuadIndexType;

    struct Quad {

        Quad();

        Quad(QuadIndexType i0, QuadIndexType i1, QuadIndexType i2, QuadIndexType i3);

        QuadIndexType i0, i1, i2, i3;
    };

    /// \class  DualMC
    /// \author Dominik Wodniok
    /// \date   2009
    /// Class which implements the dual marching cubes algorithm from Gregory M. Nielson.
    /// Faces and vertices of the standard marching cubes algorithm correspond to
    /// vertices and faces in the dual algorithm. As a vertex in standard marching cubes
    /// usually is shared by 4 faces, the dual mesh is entirely made from quadrangles.
    /// Unfortunately, under rare circumstances the original algorithm can create
    /// non-manifold meshes. See the remarks of the original paper on this.
    /// The class optionally can guarantee manifold meshes by taking the Manifold
    /// Dual Marching Cubes approach from Rephael Wenger as described in
    /// chapter 3.3.5 of his book "Isosurfaces: Geometry, Topology, and Algorithms".
    template<class T> 
    class MarchingCubes {
    public:
        // typedefs
        typedef T VolumeDataType;

        void build(
            VolumeDataType const* data,
            int32 const dimX, int32 const dimY, int32 const dimZ,
            VolumeDataType const iso,
            bool const generateManifold,
            bool const generateSoup,
            std::vector<Vector3f>& vertices,
            std::vector<Quad>& quads
        );

    private:

        void buildSharedVerticesQuads(
            VolumeDataType const iso,
            std::vector<Vector3f>& vertices,
            std::vector<Quad>& quads
        );

        void buildQuadSoup(
            VolumeDataType const iso,
            std::vector<Vector3f>& vertices,
            std::vector<Quad>& quads
        );

    private:

        /// enum with edge codes for a 12-bit voxel edge mask to indicate
        /// grid edges which intersect the ISO surface of classic marching cubes
        enum DMCEdgeCode {
            EDGE0 = 1,
            EDGE1 = 1 << 1,
            EDGE2 = 1 << 2,
            EDGE3 = 1 << 3,
            EDGE4 = 1 << 4,
            EDGE5 = 1 << 5,
            EDGE6 = 1 << 6,
            EDGE7 = 1 << 7,
            EDGE8 = 1 << 8,
            EDGE9 = 1 << 9,
            EDGE10 = 1 << 10,
            EDGE11 = 1 << 11,
            FORCE_32BIT = 0xffffffff
        };

        /// get the 8-bit in-out mask for the voxel corners of the cell cube at (cx,cy,cz)
        /// and the given iso value
        int getCellCode(int32 const cx, int32 const cy, int32 const cz, VolumeDataType const iso) const;

        /// Get the 12-bit dual point code mask, which encodes the traditional
        /// marching cube vertices of the traditional marching cubes face which
        /// corresponds to the dual point.
        /// This is also where the manifold dual marching cubes algorithm is
        /// implemented.
        int getDualPointCode(int32 const cx, int32 const cy, int32 const cz,
            VolumeDataType const iso, DMCEdgeCode const edge) const;

        /// Given a dual point code and iso value, compute the dual point.
        void calculateDualPoint(int32 const cx, int32 const cy, int32 const cz,
            VolumeDataType const iso, int const pointCode, Vector3f& v) const;

        /// Get the shared index of a dual point which is uniquly identified by its
        /// cell cube index and a cube edge. The dual point is computed,
        /// if it has not been computed before.
        QuadIndexType getSharedDualPointIndex(int32 const cx, int32 const cy, int32 const cz,
            VolumeDataType const iso, DMCEdgeCode const edge,
            std::vector<Vector3f>& vertices);

        /// Compute a linearized cell cube index.
        int32_t gA(int32 const x, int32 const y, int32 const z) const;

    private:
        // static lookup tables needed for (manifold) dual marching cubes

        /// Dual Marching Cubes table
        /// Encodes the edge vertices for the 256 marching cubes cases.
        /// A marching cube case produces up to four faces and ,thus, up to four
        /// dual points.
        static int32 const dualPointsList[256][4];

        /// Table which encodes the ambiguous face of cube configurations, which
        /// can cause non-manifold meshes.
        /// Needed for manifold dual marching cubes.
        static uint8 const problematicConfigs[256];

    private:

        /// convenience volume extent array for x-,y-, and z-dimension
        int32 dims[3];

        /// convenience volume data point
        VolumeDataType const* data;

        /// store whether the manifold dual marching cubes algorithm should be
        /// applied.
        bool generateManifold;

        /// Dual point key structure for hashing of shared vertices
        struct DualPointKey {
            // a dual point can be uniquely identified by ite linearized volume cell
            // id and point code
            int32 linearizedCellID;
            int pointCode;
            /// Equal operator for unordered map
            bool operator==(DualPointKey const& other) const;
        };

        /// Functor for dual point key hash generation
        struct DualPointKeyHash {
            size_t operator()(DualPointKey const& k) const {
                return size_t(k.linearizedCellID) | (size_t(k.pointCode) << 32u);
            }
        };

        /// Hash map for shared vertex index computations
        std::unordered_map<DualPointKey, QuadIndexType, DualPointKeyHash> pointToIndex;
    };

    inline Quad::Quad() {}

    inline Quad::Quad(
            QuadIndexType i0,
            QuadIndexType i1,
            QuadIndexType i2,
            QuadIndexType i3
        ) : i0(i0), i1(i1), i2(i2), i3(i3) {}

    template<class T> inline
        int32_t MarchingCubes<T>::gA(int32 const x, int32 const y, int32 const z) const {
        return x + dims[0] * (y + dims[1] * z);
    }

    //------------------------------------------------------------------------------
    template<class T> inline
        bool MarchingCubes<T>::DualPointKey::operator==(typename MarchingCubes<T>::DualPointKey const& other) const {
        return linearizedCellID == other.linearizedCellID && pointCode == other.pointCode;
    }
}

#include "MarchingCubes.inl"

#include "MarchingCubesTables.inl"

#endif // !MARCHING_CUBES_H__