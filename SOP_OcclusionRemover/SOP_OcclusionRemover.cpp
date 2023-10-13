#include <SOP/SOP_Node.h>
#include <SOP/SOP_API.h>
#include <SOP/SOP_GraphProxy.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <UT/UT_StringHolder.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_TemplateBuilder.h>
#include <UT/UT_DSOVersion.h>
#include <GA/GA_Handle.h>
#include <GA/GA_SplittableRange.h>
#include <GA/GA_Types.h>
#include <SIM/SIM_Random.h>
#include <tbb/pipeline.h>
#include <embree3/rtcore.h>
RTC_NAMESPACE_OPEN
class PRM_Template;
class SOP_OcclusionRemover : public SOP_Node
{
public:
    static PRM_Template *buildTemplates();
    static OP_Node *myConstructor(OP_Network *net, const char* name, OP_Operator* op)
    {
        return new SOP_OcclusionRemover(net, name, op);
    }
    const SOP_NodeVerb* cookVerb() const override;
protected:
    SOP_OcclusionRemover(OP_Network* net, const char* name, OP_Operator *op)
        : SOP_Node(net, name, op)
    {
        mySopFlags.setManagesDataIDs(true);
    }
    ~SOP_OcclusionRemover() override {}
    OP_ERROR cookMySop(OP_Context& context) override
    {
        return cookMyselfAsVerb(context);
    }
    const char* inputLabel(unsigned idx) const override
    {
        switch (idx)
        {
            case 0:     return "Occludee";
            case 1:     return "Occluder";
            case 2:     return "Visible Area";
            default:    return "Invalid Source";
        }
    }

    int isRefInput(unsigned i) const override
    {
        return true;
    }
};

using namespace UT::Literal;

class SOP_OcclusionRemoverParms : public SOP_NodeParms
{
public:
    static int version() { return 1; }
    SOP_OcclusionRemoverParms()
    {

    }
    ~SOP_OcclusionRemoverParms() override {}
    explicit SOP_OcclusionRemoverParms(const SOP_OcclusionRemoverParms&) = default;
    void loadFromOpSubclass(const LoadParms& loadparms) override
    {}
    void copyFrom(const SOP_NodeParms* src) override
    {
        *this = *((const SOP_OcclusionRemoverParms*)src);
    }
    template <typename T>
	void doGetParmValue(TempIndex idx, TempIndex instance, T& value) const
	{
		if (idx.size() < 1)
		{
			return;
		}
		UT_ASSERT(idx.size() == instance.size());
		if (idx.size() != instance.size() + 1)
		{
			return;
		}
		switch (idx[0])
		{
		case 0:
			coerceValue(value, myAttrib);
			break;
		case 1:
			coerceValue(value, numRay);
			break;
		case 2:
			coerceValue(value, numRandomSampleForEachPrimitive);
			break;
		case 3:
			coerceValue(value, parallelism);
			break;
		}
	}

	void getNestParmValue(TempIndex idx, TempIndex instance, exint& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, fpreal& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Vector2D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Vector3D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Vector4D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Matrix2D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Matrix3D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_Matrix4D& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_StringHolder& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, UT_SharedPtr<UT_Ramp>& value) const override
	{
		doGetParmValue(idx, instance, value);
	}
	void getNestParmValue(TempIndex idx, TempIndex instance, PRM_DataItemHandle& value) const override
	{
		doGetParmValue(idx, instance, value);
	}

    exint getNestNumParms(TempIndex idx) const override
	{
		if (idx.size() == 0)
		{
			return 8;
		}
		switch (idx[0])
		{

		}
		// Invalid
		return 0;
	}

	const char* getNestParmName(TempIndex fieldnum) const override
	{
		if (fieldnum.size() < 1)
		{
			return 0;
		}
		switch (fieldnum[0])
		{
		case 0:
			return "attrib";
		case 1:
			return "numray";
		case 2:
			return "numrandomsample";
		case 3:
			return "parallelism";
		}
		return 0;
	}

	ParmType getNestParmType(TempIndex fieldnum) const override
	{
		if (fieldnum.size() < 1)
			return PARM_UNSUPPORTED;
		switch (fieldnum[0])
		{
		case 0:
			return PARM_STRING;
		case 1:
		case 2:
		case 3:
			return PARM_INTEGER;
		}
		return PARM_UNSUPPORTED;
	}

	const UT_StringHolder& getAttrib() const { return myAttrib; }
	void setAttrib(const UT_StringHolder& val) { myAttrib = val; }
	int getNumRay() const { return numRay; }
	void setNumRay(int x) { numRay = x; }
	int getNumRandomSampleForEachPrimitive() const { return numRandomSampleForEachPrimitive; }
	void setNumRandomSampleForEachPrimitive(int x) { numRandomSampleForEachPrimitive = x; }
	int getParallelism() const { return parallelism; }
	void setParallelism(int value) { parallelism = value; }
private:
	UT_StringHolder myAttrib = "visibility"_sh;
	int numRay = 10;
	int numRandomSampleForEachPrimitive = 10;
	int parallelism = 4;
};

struct RayHit
{
	bool isVisible;
	int hitPrim;
	float hitU, hitV;
	UT_Vector3 hitPos;
};

class RayTracingAccelerationStructure
{
private:
	RTCScene _scene;
	RTCGeometry _occluder;
	RTCGeometry _visibleArea;
	UT_Array<float> _occluderVertices;
	UT_Array<uint> _occluderIndices;
	UT_Array<float> _visibleAreaVertices;
	UT_Array<uint> _visibleAreaIndices;
	const int kOccluderId = 1;
	const int kVisibleAreaId = 2;
	bool _initialized = false;
public:
	UT_Array<float>* GetOccluderVerticesRef()
	{
		if (!_initialized) return nullptr;
		return &_occluderVertices;
	}
	UT_Array<uint>* GetOccluderIndicesRef()
	{
		if (!_initialized) return nullptr;
		return &_occluderIndices;
	}
	UT_Array<float>* GetVisibleAreaVerticesRef()
	{
		if (!_initialized) return nullptr;
		return &_visibleAreaVertices;
	}
	UT_Array<uint>* GetVisibleAreaIndicesRef()
	{
		if (!_initialized) return nullptr;
		return &_visibleAreaIndices;
	}
	void Initialize(RTCDevice& pDevice, const GA_Detail& occluder, const GA_Detail& visibleArea)
	{
		if (_initialized)
		{
			Destroy();
		}
		_scene = rtcNewScene(pDevice);
		rtcSetSceneFlags(_scene, RTC_SCENE_FLAG_ROBUST);
		FillMeshBuffer(_occluderVertices, _occluderIndices, occluder);
		FillMeshBuffer(_visibleAreaVertices, _visibleAreaIndices, visibleArea);
		_occluder = rtcNewGeometry(pDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
		_visibleArea = rtcNewGeometry(pDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
		rtcSetSharedGeometryBuffer(
			_occluder,
			RTC_BUFFER_TYPE_VERTEX,
			0,
			RTC_FORMAT_FLOAT3,
			_occluderVertices.data(),
			0, sizeof(float) * 3, _occluderVertices.size()
		);
		rtcSetSharedGeometryBuffer(
			_occluder,
			RTC_BUFFER_TYPE_INDEX,
			0,
			RTC_FORMAT_UINT3,
			_occluderIndices.data(),
			0, sizeof(int) * 3, _occluderIndices.size() / 3
		);
		rtcSetSharedGeometryBuffer(
			_visibleArea,
			RTC_BUFFER_TYPE_VERTEX,
			0,
			RTC_FORMAT_FLOAT3,
			_visibleAreaVertices.data(),
			0, sizeof(float) * 3, _visibleAreaVertices.size());
		rtcSetSharedGeometryBuffer(
			_visibleArea,
			RTC_BUFFER_TYPE_INDEX,
			0,
			RTC_FORMAT_UINT3,
			_visibleAreaIndices.data(),
			0, sizeof(uint) * 3, _visibleAreaIndices.size() / 3
		);
		rtcCommitGeometry(_occluder);
		rtcCommitGeometry(_visibleArea);
		rtcAttachGeometryByID(_scene, _occluder, kOccluderId);
		rtcAttachGeometryByID(_scene, _visibleArea, kVisibleAreaId);
		rtcCommitScene(_scene);
		_initialized = true;
	}
	void Destroy()
	{
		_occluderVertices.clear();
		_occluderIndices.clear();
		_visibleAreaVertices.clear();
		_visibleAreaIndices.clear();
		rtcReleaseGeometry(_occluder);
		rtcReleaseGeometry(_visibleArea);
		rtcReleaseScene(_scene);
	}
	bool IsInitialized()
	{
		return _initialized;
	}
	void FillMeshBuffer(UT_Array<float>& vertices, UT_Array<uint>& indices, const GA_Detail& geom)
	{
		UT_Array<int> primIndicesBuffer;
		for (int primId = 0; primId < geom.getNumPrimitives(); ++primId)
		{
			primIndicesBuffer.clear();
			auto prim = geom.getPrimitiveByIndex(primId);
			prim->forEachPoint(
				[&](int pid)
				{
					primIndicesBuffer.append(pid);
				});
			if (primIndicesBuffer.size() == 3)
			{
				for (int i = 0; i < 3; ++i)
				{
					indices.append(primIndicesBuffer[i]);
				}
			}
			else if (primIndicesBuffer.size() > 3)
			{
				int a = primIndicesBuffer[0];
				int b = primIndicesBuffer[1];
				for (int i = 2; i < primIndicesBuffer.size(); ++i)
				{
					indices.append(a);
					indices.append(b);
					indices.append(primIndicesBuffer[i]);
					b = primIndicesBuffer[i];
				}
			}
		}
		GA_Offset rangeBegin, rangeEnd;
		for (GA_Iterator it(geom.getPointRange()); it.blockAdvance(rangeBegin, rangeEnd);)
		{
			for (auto ptOff = rangeBegin; ptOff < rangeEnd; ++ptOff)
			{
				auto P = geom.getPos3(ptOff);
				vertices.append(P.x());
				vertices.append(P.y());
				vertices.append(P.z());
			}
		}
	}
	
	void QueryRayHit(float ox, float oy, float oz, float dx, float dy, float dz, float near, float far, RayHit* pHit)
	{
		RTCRayHit hit = RTCRayHit();
		hit.ray.id = hit.ray.mask = hit.ray.time = hit.ray.flags = 0;
		hit.ray.org_x = ox;
		hit.ray.org_y = oy;
		hit.ray.org_z = oz;
		hit.ray.dir_x = dx;
		hit.ray.dir_y = dy;
		hit.ray.dir_z = dz;
		hit.ray.tnear = near;
		hit.ray.tfar = far;
		hit.ray.flags = 0;
		hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
		
		RTCIntersectContext context;
		rtcInitIntersectContext(&context);
		rtcIntersect1(_scene, &context, &hit);
		if (hit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
		{
			pHit->isVisible = hit.hit.geomID == kVisibleAreaId;
		}
		else
		{
			pHit->isVisible = false;
		}
		pHit->hitPrim = hit.hit.primID;
		pHit->hitU = hit.hit.u;
		pHit->hitV = hit.hit.v;
		pHit->hitPos = UT_Vector3(ox, oy, oz) + hit.ray.tfar * UT_Vector3(dx, dy, dz);
	}
};

class SOP_OcclusionRemoverCache : public SOP_NodeCache
{
private:
	RTCDevice _device;
	RayTracingAccelerationStructure _as;

	struct CacheVersionKey
	{
		bool valid = false;
		GA_DataId topologyDataId;
		GA_DataId primListDataId;
		GA_DataId PDataId;
		CacheVersionKey() {}
		CacheVersionKey(GA_DataId topologyId, GA_DataId primId, GA_DataId pId)
			: topologyDataId(topologyId), primListDataId(primId), PDataId(pId)
		{}
		bool operator == (const CacheVersionKey& rhs) const
		{
			return topologyDataId == rhs.topologyDataId
				&& primListDataId == rhs.primListDataId
				&& PDataId == rhs.PDataId;
		}
	} _occluderCacheKey, _visibleAreaCacheKey;
public:
    SOP_OcclusionRemoverCache() : SOP_NodeCache()
    {
		_device = rtcNewDevice(nullptr);
	}
    virtual ~SOP_OcclusionRemoverCache() 
	{
		if (_as.IsInitialized())
		{
			_as.Destroy();
		}
		rtcReleaseDevice(_device);
	}
	
	void EnsureCache(const GA_Detail& occluder, const GA_Detail& visibleArea)
	{
		CacheVersionKey occluderCacheKey = CacheVersionKey(
			occluder.getTopology().getDataId(),
			occluder.getPrimitiveList().getDataId(),
			occluder.getP()->getDataId());
		CacheVersionKey visibleAreaCacheKey = CacheVersionKey(
			visibleArea.getTopology().getDataId(),
			visibleArea.getPrimitiveList().getDataId(),
			visibleArea.getP()->getDataId());
		if (occluderCacheKey == _occluderCacheKey && visibleAreaCacheKey == _visibleAreaCacheKey)
		{
			return;
		}
		_occluderCacheKey = occluderCacheKey;
		_visibleAreaCacheKey = visibleAreaCacheKey;
		_as.Initialize(_device, occluder, visibleArea);
	}

	RayTracingAccelerationStructure& GetRayTracingAccelerationStructureRef()
	{
		return _as;
	}

	/// <summary>
	/// return pointer to cached occluder vertices buffer.
	/// return nullptr if cache is not prepared.
	/// </summary>
	/// <returns></returns>
	UT_Array<float>* GetOccluderVerticesRef()
	{
		return _as.GetOccluderVerticesRef();
	}
	/// <summary>
	/// return pointer to cached occluder indices buffer.
	/// return nullptr if cache is not prepared.
	/// </summary>
	/// <returns></returns>
	UT_Array<uint>* GetOccluderIndicesRef()
	{
		return _as.GetOccluderIndicesRef();
	}
	/// <summary>
	/// return pointer to cached visible area vertices buffer.
	/// return nullptr if cache is not prepared.
	/// </summary>
	/// <returns></returns>
	UT_Array<float>* GetVisibleAreaVerticesRef()
	{
		return _as.GetVisibleAreaVerticesRef();
	}
	/// <summary>
	/// return pointer to cached visible area indices buffer.
	/// return nullptr if cache is not prepared.
	/// </summary>
	/// <returns></returns>
	UT_Array<uint>* GetVisibleAreaIndicesRef()
	{
		return _as.GetVisibleAreaIndicesRef();
	}
};

struct Ray
{
	int primId;
	float ox, oy, oz;
	float dx, dy, dz;
	bool visible;
	int hitPrim;
	float hitU, hitV;
	UT_Vector3 hitPos;
public:
	static Ray* Allocate()
	{
		Ray* ray = (Ray*)tbb::tbb_allocator<char>().allocate(sizeof(Ray));
		return ray;
	}
	void Free()
	{
		tbb::tbb_allocator<char>().deallocate((char*)this, sizeof(Ray));
	}
	void SetRayOrigin(float x, float y, float z)
	{
		ox = x;
		oy = y;
		oz = z;
	}
	void SetRayDirection(float x, float y, float z)
	{
		dx = x;
		dy = y;
		dz = z;
	}
};

const float epsilon = 1e-6;
class RayGeneratorCache
{
private:
	const GEO_Detail* _pGeom;
	bool _initialized = false;
	int _primNum;
	UT_Array<int> _primIndicesBuffer;
	GA_ROHandleV3 _primNormalAttr;

	UT_Array<Ray*> _rays;
	int _currentCursor = 0;
	int _currentPrim = 0;
	int _numRay;
	int _numRandomSampleCount;
public:
	void Initialize(const GEO_Detail& geom, int numRay, int numRandomSampleCount)
	{
		_currentPrim = 0;
		_pGeom = &geom;
		_primNum = geom.getNumPrimitives();
		_initialized = true;
		_numRay = numRay;
		_numRandomSampleCount = numRandomSampleCount;
		_primNormalAttr = GA_ROHandleV3(geom.findFloatTuple(GA_ATTRIB_PRIMITIVE, "Normal"));
	}
	bool IsInitialized()
	{
		return _initialized;
	}
	void ClearRayBuffer()
	{
		_rays.clear();
		_currentCursor = 0;
	}

	double RadicalInverse(int base, int i)
	{
		int pointCoef = 1;
		int inverse = 0;
		for (; i > 0; i /= base)
		{
			inverse = inverse * base + (i % base);
			pointCoef = pointCoef * base;
		}
		return inverse / (double)pointCoef;
	}
	double Hammersley(int dimension, int index, int numSamples)
	{
		static double primTable[] = { 2, 3, 5 };
		if (dimension == 0)
		{
			return index / (double)numSamples;
		}
		else
		{
			UT_ASSERT(dimension - 1 < 3);
			return RadicalInverse(primTable[dimension - 1], index);
		}
	}
	uint32_t InverseBase4(uint32_t value)
	{
		value = ((value & 0xFFFF0000) >> 16) | ((value & 0x0000FFFF) << 16);
		value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
		value = ((value & 0xF0F0F0F0) >> 4) | ((value & 0x0F0F0F0F) << 4);
		value = ((value & 0xCCCCCCCC) >> 2) | ((value & 0x33333333) << 2);
		value = ((value & 0xAAAAAAAA) >> 1) | ((value & 0x55555555) << 1);
		return value;
	}
	/// <summary>
	/// code adapt from:
	/// https://pharr.org/matt/blog/2019/02/27/triangle-sampling-1
	/// </summary>
	UT_Vector3 BasuOwenMapping(uint32_t value)
	{
		UT_Vector2 A, B, C, An, Bn, Cn;
		A = UT_Vector2(1, 0);
		B = UT_Vector2(0, 1);
		C = UT_Vector2(0, 0);
		for (int i = 0; i < 16; ++i)
		{
			uint32_t d = (value & 0x3);
			switch (d) {
			case 0:
				An = (B + C) / 2;
				Bn = (A + C) / 2;
				Cn = (A + B) / 2;
				break;
			case 1:
				An = A;
				Bn = (A + B) / 2;
				Cn = (A + C) / 2;
				break;
			case 2:
				An = (B + A) / 2;
				Bn = B;
				Cn = (B + C) / 2;
				break;
			case 3:
				An = (C + A) / 2;
				Bn = (C + B) / 2;
				Cn = C;
				break;
			}
			A = An;
			B = Bn;
			C = Cn;
			value >>= 2;
		}
		UT_Vector2 r2 = (A + B + C) / 3;
		return UT_Vector3(r2.x(), r2.y(), 1 - r2.x() - r2.y());
	}
	UT_Vector3 Transform(const UT_Matrix4& mat, const UT_Vector3& pos)
	{
		auto raw = mat.data();
		auto x = raw[0] * pos[0] + raw[1] * pos[1] + raw[2] * pos[2] + raw[3] * 1;
		auto y = raw[4] * pos[0] + raw[5] * pos[1] + raw[6] * pos[2] + raw[7] * 1;
		auto z = raw[8] * pos[0] + raw[9] * pos[1] + raw[10] * pos[2] + raw[11] * 1;
		auto w = raw[12] * pos[0] + raw[13] * pos[1] + raw[14] * pos[2] + raw[15] * 1;
		UT_Vector3 result = UT_Vector3(x / w, y / w, z / w);
		return result;
	}
	void GenerateHemisphereRays(UT_Vector3 pos, UT_Vector3 normal, UT_Matrix4 local2World, int numRay)
	{
		constexpr float PI = 3.141592653589793238;
		pos += normal * epsilon;
		for (int i = 0; i < numRay; ++i)
		{
			double u = Hammersley(0, i, numRay);
			double v = Hammersley(1, i, numRay);
			double r = std::sqrt(1.0 - u * u);
			double phi = 2 * PI * v;
			auto sample = UT_Vector3(std::cos(phi) * r, u, std::sin(phi) * r);
			sample = Transform(local2World, sample);
			UT_Vector3 dir = sample;
			dir.normalize();
			Ray* ray = Ray::Allocate();
			ray->ox = pos.x();
			ray->oy = pos.y();
			ray->oz = pos.z();
			ray->dx = dir.x();
			ray->dy = dir.y();
			ray->dz = dir.z();
			ray->primId = _currentPrim;
			_rays.append(ray);
		}
	}
	void SetupSampleRaysForEachTriangle()
	{
		if (_primIndicesBuffer.size() < 3)
		{
			return;
		}
		int ia = _pGeom->pointOffset(_primIndicesBuffer[0]);
		int ib = _pGeom->pointOffset(_primIndicesBuffer[1]);
		for (int i = 2; i < _primIndicesBuffer.size(); ++i)
		{
			int ic = _pGeom->pointOffset(_primIndicesBuffer[i]);
			UT_Vector3 pa, pb, pc;
			pa = _pGeom->getPos3(ia);
			pb = _pGeom->getPos3(ib);
			pc = _pGeom->getPos3(ic);
			UT_Vector3 ab = pb - pa;
			ab.normalize();
			UT_Vector3 ac = pc - pa;
			ac.normalize();
			auto offset = _pGeom->primitiveOffset(_currentPrim);
			UT_Vector3 normal = _primNormalAttr.get(offset);
			normal.normalize();
			UT_Matrix4 local2World;
			float rawData[4][4];
			UT_Vector3 binormal = ab;
			binormal.cross(normal);		// inplace cross
			binormal.normalize();
			rawData[0][0] = ab.x(); rawData[0][1] = normal.x(); rawData[0][2] = binormal.x(); rawData[0][3] = 0;
			rawData[1][0] = ab.y(); rawData[1][1] = normal.y(); rawData[1][2] = binormal.y(); rawData[1][3] = 0;
			rawData[2][0] = ab.z(); rawData[2][1] = normal.z(); rawData[2][2] = binormal.z(); rawData[2][3] = 0;
			rawData[3][0] = rawData[3][1] = rawData[3][2] = 0; rawData[3][3] = 1;
			local2World = UT_Matrix4(rawData);
			//local2World.transpose();
			UT_Vector3 centerPos = (pa + pb + pc) / 3;
			GenerateHemisphereRays(pa, normal, local2World, _numRay);
			GenerateHemisphereRays(pb, normal, local2World, _numRay);
			GenerateHemisphereRays(pc, normal, local2World, _numRay);
			GenerateHemisphereRays(centerPos, normal, local2World, _numRay);
			
			for (int r = 0; r < _numRandomSampleCount; ++r)
			{
				auto triangleCode = InverseBase4(r);
				UT_Vector3 sample = BasuOwenMapping(triangleCode);
				sample = sample.x() * pa + sample.y() * pb + sample.z() * pc;
				GenerateHemisphereRays(sample, normal, local2World, _numRay);
			}
			ib = ic;
		}
	}
	void SetupSampleRaysForCurrentPrim()
	{
		_primIndicesBuffer.clear();
		auto prim = _pGeom->getPrimitiveByIndex(_currentPrim);
		prim->forEachPoint(
			[&](int pid)
			{
				_primIndicesBuffer.append(pid);
			}
		);
		SetupSampleRaysForEachTriangle();
	}
	void GoToNextPrim()
	{
		++_currentPrim;
	}
	bool IsEndOfPrimitives()
	{
		return _currentPrim == _primNum;
	}
	Ray* PopRay()
	{
		UT_ASSERT(_currentCursor < _rays.size());
		return _rays[_currentCursor++];
	}
	bool HasRay()
	{
		return _currentCursor < _rays.size();
	}
};

RayGeneratorCache g_rayGeneratorCache;
class RayGenerator
{
private:
	const GEO_Detail* _pGeom;
	int _numRay;
	int _numRandomSampleCount;
public:
	RayGenerator(const GEO_Detail& geom, int numRay, int numRandomSampleCount) : _pGeom(&geom), _numRay(numRay), _numRandomSampleCount(numRandomSampleCount)
	{}
	~RayGenerator()
	{}
	RayGenerator(const RayGenerator& generator) : _pGeom(generator._pGeom), _numRay(generator._numRay), _numRandomSampleCount(generator._numRandomSampleCount)
	{}
	Ray* operator()(tbb::flow_control& fc) const
	{
		if (g_rayGeneratorCache.HasRay())
		{
			return g_rayGeneratorCache.PopRay();
		}
		if (!g_rayGeneratorCache.IsInitialized())
		{
			g_rayGeneratorCache.Initialize(*_pGeom, _numRay, _numRandomSampleCount);
		}
		
		if (g_rayGeneratorCache.IsEndOfPrimitives())
		{
			g_rayGeneratorCache = RayGeneratorCache();		// initialized global cache
			fc.stop();
			return nullptr;
		}
		g_rayGeneratorCache.SetupSampleRaysForCurrentPrim();
		g_rayGeneratorCache.GoToNextPrim();
		// Note: now we assume it should have at least one ray on primitive
		return g_rayGeneratorCache.PopRay();
	}
};

class VisibilityTestOperator
{
private:
	RayTracingAccelerationStructure* _pAS;
public:
	VisibilityTestOperator(RayTracingAccelerationStructure& as) : _pAS(&as)
	{}
	~VisibilityTestOperator() {}
	VisibilityTestOperator(const VisibilityTestOperator& op)
	{
		_pAS = op._pAS;
	}
	Ray* operator()(Ray* ray) const
	{
		RayHit hit;
		_pAS->QueryRayHit(ray->ox, ray->oy, ray->oz, ray->dx, ray->dy, ray->dz, 0, std::numeric_limits<float>::infinity(), &hit);
		ray->visible = hit.isVisible;
		ray->hitPrim = hit.hitPrim;
		ray->hitU = hit.hitU;
		ray->hitV = hit.hitV;
		ray->hitPos = hit.hitPos;
		return ray;
	}
};

class VisibilityMarker
{
private:
	UT_Array<bool>* _visibilityOfPrim;
	// TODO: remove
	UT_Array<UT_Vector3>* _rayOrigin;
	UT_Array<UT_Vector3>* _rayDir;
	UT_Array<int>* _hitPrim;
	UT_Array<UT_Vector2>* _hitUV;
	UT_Array<bool>* _hitVisible;
	UT_Array<UT_Vector3>* _hitPos;
public:
	VisibilityMarker(
		UT_Array<bool>& visibilityOfPrim, 
		UT_Array<UT_Vector3>* pRayOrigin = nullptr, 
		UT_Array<UT_Vector3>* pRayDir = nullptr,
		UT_Array<int>* pHitPrim = nullptr,
		UT_Array<UT_Vector2>* pHitUV = nullptr,
		UT_Array<bool>* pHitVisible = nullptr,
		UT_Array<UT_Vector3>* pHitPos = nullptr
	) 
		: 
		_visibilityOfPrim(&visibilityOfPrim), 
		_rayOrigin(pRayOrigin), 
		_rayDir(pRayDir),
		_hitPrim(pHitPrim),
		_hitUV(pHitUV),
		_hitVisible(pHitVisible),
		_hitPos(pHitPos)
	{}
	VisibilityMarker(const VisibilityMarker& op) 
		: 
		_visibilityOfPrim(op._visibilityOfPrim), 
		_rayOrigin(op._rayOrigin), 
		_rayDir(op._rayDir),
		_hitPrim(op._hitPrim),
		_hitUV(op._hitUV),
		_hitVisible(op._hitVisible),
		_hitPos(op._hitPos)
	{}
	~VisibilityMarker(){}
	void operator()(Ray* ray) const
	{
		if (_visibilityOfPrim->size() < ray->primId + 1 && ray->primId >= 0)
		{
			// Implementation note: in best practice, user should pre-allocate enough space to store result. But if user don't, we resize it to make sure all the results are stored.
			int currentSize = _visibilityOfPrim->size();
			_visibilityOfPrim->setCapacity(ray->primId + 1);
			_visibilityOfPrim->setSize(ray->primId + 1);
		}
		(*_visibilityOfPrim)[ray->primId] = (*_visibilityOfPrim)[ray->primId] || ray->visible;

		// TODO: remove
		if (_rayOrigin != nullptr && _rayDir != nullptr)
		{
			(*_rayOrigin).append(UT_Vector3(ray->ox, ray->oy, ray->oz));
			(*_rayDir).append(UT_Vector3(ray->dx, ray->dy, ray->dz));
		}
		if (_hitPrim != nullptr && _hitUV != nullptr && _hitVisible != nullptr && _hitPos != nullptr)
		{
			(*_hitPrim).append(ray->hitPrim);
			(*_hitUV).append(UT_Vector2(ray->hitU, ray->hitV));
			(*_hitVisible).append(ray->visible);
			(*_hitPos).append(ray->hitPos);
		}
	}
};

class SOP_OcclusionRemoverVerb : public SOP_NodeVerb
{
public:
    static const UT_StringHolder theSOPTypeName;
    static const SOP_NodeVerb::Register<SOP_OcclusionRemoverVerb> theVerb;
    static const char* const theDsFile;
    virtual SOP_NodeParms* allocParms() const { return new SOP_OcclusionRemoverParms(); }
    virtual SOP_NodeCache* allocCache() const { return new SOP_OcclusionRemoverCache(); }
    virtual UT_StringHolder name() const override { return theSOPTypeName; }
    virtual CookMode cookMode(const SOP_NodeParms* parms) const { return COOK_INPLACE; }
    virtual void cook(const CookParms& cookParms) const;
};

const UT_StringHolder SOP_OcclusionRemoverVerb::theSOPTypeName("SOP_OcclusionRemover"_sh);
const SOP_NodeVerb::Register<SOP_OcclusionRemoverVerb> SOP_OcclusionRemoverVerb::theVerb;

PRM_Template* SOP_OcclusionRemover::buildTemplates()
{
    static PRM_TemplateBuilder templ("SOP_OcclusionRemover.cpp"_sh, SOP_OcclusionRemoverVerb::theDsFile);
    return templ.templates();
}

const SOP_NodeVerb* SOP_OcclusionRemover::cookVerb() const
{
    return SOP_OcclusionRemoverVerb::theVerb.get();
}

const char* const SOP_OcclusionRemoverVerb::theDsFile = R"THEDSFILE(
    {
        name occlusionremover
		parm {
		name "attribname"
		label "Attribute Name"
		type string
		default { "visibility" }
		}
		parm {
		name "numray"
		label "Num of rays"
		type integer
		default { 20 }
		}
		parm {
		name "num_random_sample"
		label "Primitive random sample"
		type integer
		default { 10 }
		}
		parm {
		name "parallelism"
		label "Parallelism"
		type integer
		default { 16 }
		}
    })THEDSFILE";

void SOP_OcclusionRemoverVerb::cook(const CookParms& cookparms) const
{
    auto&& sopParms = cookparms.parms<SOP_OcclusionRemoverParms>();
    auto sopCache = (SOP_OcclusionRemoverCache*)cookparms.cache();
	GEO_Detail* const occludee = cookparms.gdh().gdpNC();
	const GEO_Detail* const occluder = cookparms.hasInput(1) ? cookparms.inputGeo(1) : nullptr;
	const GEO_Detail* const visibleArea = cookparms.hasInput(2) ? cookparms.inputGeo(2) : nullptr;
	sopCache->EnsureCache(*occluder, *visibleArea);
	
	GA_RWHandleV3 normalAttr = occludee->findFloatTuple(GA_ATTRIB_PRIMITIVE, "Normal");
	//if (!normalAttr.isValid())
	//{
	//	normalAttr.bind(occludee->addFloatTuple(GA_ATTRIB_PRIMITIVE, "Normal", 3));
	//}
	if (!normalAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, "Normal");
		return;
	}
	//occludee->normal(normalAttr);

	// TODO: remove debug code
	UT_Array<UT_Vector3> rayOrigin;
	UT_Array<UT_Vector3> rayDir;
	UT_Array<bool> hitVisible;
	UT_Array<int> hitPrim;
	UT_Array<UT_Vector2> hitUV;
	UT_Array<UT_Vector3> hitPos;

	UT_Array<bool> visibility;
	visibility.setCapacity(occludee->getNumPrimitives());
	visibility.setSize(occludee->getNumPrimitives());
	for (int i = 0; i < occludee->getNumPrimitives(); ++i)
	{
		visibility[i] = false;
	}
	tbb::parallel_pipeline(
		sopParms.getParallelism(),
		tbb::make_filter<void, Ray*>(
			tbb::filter::serial_in_order, RayGenerator(*occludee, sopParms.getNumRay(), sopParms.getNumRandomSampleForEachPrimitive())
			)
		&
		tbb::make_filter<Ray*, Ray*>(
			tbb::filter::parallel, VisibilityTestOperator(sopCache->GetRayTracingAccelerationStructureRef())
			)
		&
		tbb::make_filter<Ray*, void>(
			tbb::filter::serial_out_of_order, VisibilityMarker(visibility, &rayOrigin, &rayDir, &hitPrim, &hitUV, &hitVisible, &hitPos)
			)
	);

	GA_RWHandleI visibilityAttrib(occludee->findIntTuple(GA_ATTRIB_PRIMITIVE, sopParms.getAttrib()));
	if (!visibilityAttrib.isValid())
	{
		visibilityAttrib.bind(occludee->addIntTuple(GA_ATTRIB_PRIMITIVE, sopParms.getAttrib(), 1));
	}
	if (!visibilityAttrib.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	for (int i = 0; i < occludee->getNumPrimitives(); ++i)
	{
		auto offset = occludee->primitiveOffset(i);
		visibilityAttrib.set(offset, visibility[i]);
	}

	// TODO: remove debug
	auto block = occludee->appendPointBlock(rayOrigin.size());
	GA_RWHandleV3 rayOriginAttr(occludee->findFloatTuple(GA_ATTRIB_POINT, "ro"));
	if (!rayOriginAttr.isValid())
	{
		rayOriginAttr.bind(occludee->addFloatTuple(GA_ATTRIB_POINT, "ro", 3));
	}
	if (!rayOriginAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	GA_RWHandleV3 rayDirAttr(occludee->findFloatTuple(GA_ATTRIB_POINT, "rd"));
	if (!rayDirAttr.isValid())
	{
		rayDirAttr.bind(occludee->addFloatTuple(GA_ATTRIB_POINT, "rd", 3));
	}
	if (!rayDirAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	GA_RWHandleI hitPrimAttr(occludee->findIntTuple(GA_ATTRIB_POINT, "hit_prim"));
	if (!hitPrimAttr.isValid())
	{
		hitPrimAttr.bind(occludee->addIntTuple(GA_ATTRIB_POINT, "hit_prim", 1));
	}
	if (!hitPrimAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	GA_RWHandleV2 hitUVAttr(occludee->findFloatTuple(GA_ATTRIB_POINT, "hit_uv"));
	if (!hitUVAttr.isValid())
	{
		hitUVAttr.bind(occludee->addFloatTuple(GA_ATTRIB_POINT, "hit_uv", 2));
	}
	if (!hitUVAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	GA_RWHandleI hitVisibleAttr(occludee->findIntTuple(GA_ATTRIB_POINT, "hit_visible"));
	if (!hitVisibleAttr.isValid())
	{
		hitVisibleAttr.bind(occludee->addIntTuple(GA_ATTRIB_POINT, "hit_visible", 1));
	}
	if (!hitVisibleAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	GA_RWHandleV3 hitPosAttr(occludee->findFloatTuple(GA_ATTRIB_POINT, "hit_pos"));
	if (!hitPosAttr.isValid())
	{
		hitPosAttr.bind(occludee->addFloatTuple(GA_ATTRIB_POINT, "hit_pos", 3));
	}
	if (!hitPosAttr.isValid())
	{
		cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, sopParms.getAttrib());
		return;
	}
	for (int i = 0; i < rayOrigin.size(); ++i)
	{
		auto offset = block + i;
		rayOriginAttr.set(offset, rayOrigin[i]);
		rayDirAttr.set(offset, rayDir[i]);
		hitPrimAttr.set(offset, hitPrim[i]);
		hitUVAttr.set(offset, hitUV[i]);
		hitVisibleAttr.set(offset, hitVisible[i]);
		hitPosAttr.set(offset, hitPos[i]);
	}

	//normalAttr.clear();
	//occludee->destroyAttribute(GA_ATTRIB_PRIMITIVE, "Normal");
}

void newSopOperator(OP_OperatorTable* table)
{
    table->addOperator(new OP_Operator(
        SOP_OcclusionRemoverVerb::theSOPTypeName,
        "Occlusion Remover",
        SOP_OcclusionRemover::myConstructor,
        SOP_OcclusionRemover::buildTemplates(),
        3,
        3,
        nullptr, 
        0
    ));
}

