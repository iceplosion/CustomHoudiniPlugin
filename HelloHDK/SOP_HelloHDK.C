
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
#include <embree3/rtcore.h>

class PRM_Template;

class SOP_HelloHDK : public SOP_Node
{
public:
    static PRM_Template *buildTemplates();
    static OP_Node *myConstructor(OP_Network *net, const char *name, OP_Operator *op)
    {
        return new SOP_HelloHDK(net, name, op);
    }
    const SOP_NodeVerb* cookVerb() const override;
protected:
    SOP_HelloHDK(OP_Network* net, const char *name, OP_Operator *op) 
        : SOP_Node(net, name, op)
    {
        mySopFlags.setManagesDataIDs(true);
    }
    ~SOP_HelloHDK() override {}

    OP_ERROR cookMySop(OP_Context &context) override
    {
        return cookMyselfAsVerb(context);
    }

    const char* inputLabel(unsigned idx) const override
    {
        switch (idx)
        {
        case 0:     return "Input Mesh";
        default:    return "Invalid Source";
        }
    }

    int isRefInput(unsigned i) const override
    {
        return false;
    }
};


using namespace UT::Literal;

	class SOP_HelloHDKParms : public SOP_NodeParms
	{
	public:
		static int version() { return 1; }
		SOP_HelloHDKParms()
		{
			_segmentCount = 3;
		}

		explicit SOP_HelloHDKParms(const SOP_HelloHDKParms&) = default;

		~SOP_HelloHDKParms() override {}

		bool operator==(const SOP_HelloHDKParms& src) const
		{
			if (_segmentCount != src._segmentCount)
			{
				return false;
			}
			return true;
		}

		bool operator!=(const SOP_HelloHDKParms& src) const
		{
			return !operator==(src);
		}

		void buildFromOp(const SOP_GraphProxy* graph, exint nodeidx, fpreal time, DEP_MicroNode* depnode)
		{
			_segmentCount = 3;  // default value
			if (true)
			{
				graph->evalOpParm(_segmentCount, nodeidx, "segmentcount", time, 0);
			}
		}

		void loadFromOpSubclass(const LoadParms& loadparms) override
		{
			buildFromOp(loadparms.graph(), loadparms.nodeIdx(), loadparms.context().getTime(), loadparms.depnode());
		}

		void copyFrom(const SOP_NodeParms* src) override
		{
			*this = *((const SOP_HelloHDKParms*)src);
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
				coerceValue(value, _segmentCount);
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
				return "segmentcount";
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
				return PARM_INTEGER;
			}
			return PARM_UNSUPPORTED;
		}

		int64 getSegmentCount() const
		{
			return _segmentCount;
		}

	private:
		int64 _segmentCount;
	};

	class SOP_HelloHDKCache : public SOP_NodeCache
	{
	public:
		SOP_HelloHDKCache() : SOP_NodeCache()
		{}
		virtual ~SOP_HelloHDKCache() {}
	};

	class SOP_HelloHDKVerb : public SOP_NodeVerb
	{
	public:
		static const UT_StringHolder theSOPTypeName;
		static const SOP_NodeVerb::Register<SOP_HelloHDKVerb> theVerb;
		static const char* const theDsFile;

		virtual SOP_NodeParms* allocParms() const { return new SOP_HelloHDKParms(); }
		virtual SOP_NodeCache* allocCache() const { return new SOP_HelloHDKCache(); }
		virtual UT_StringHolder name() const { return theSOPTypeName; }

		virtual CookMode cookMode(const SOP_NodeParms* parms) const { return COOK_INPLACE; }
		virtual void cook(const CookParms& cookParms) const;
	};

	const UT_StringHolder SOP_HelloHDKVerb::theSOPTypeName("SOP_HelloHDK"_sh);
	const SOP_NodeVerb::Register<SOP_HelloHDKVerb> SOP_HelloHDKVerb::theVerb;

	PRM_Template* SOP_HelloHDK::buildTemplates()
	{
		static PRM_TemplateBuilder templ("SOP_HelloHDK.C"_sh, SOP_HelloHDKVerb::theDsFile);
        if (templ.justBuilt())
        {

        }
		return templ.templates();
	}

	const SOP_NodeVerb* SOP_HelloHDK::cookVerb() const
	{
		return SOP_HelloHDKVerb::theVerb.get();
	}

	const char *const SOP_HelloHDKVerb::theDsFile = R"THEDSFILE(
	{
		name parameters
		parm {
			name "segmentcount"
			cppname "SegmentCount"
			label "Segment Count"
			type integer
			default 3
		}
	})THEDSFILE";

	void SOP_HelloHDKVerb::cook(const CookParms& cookparms) const
	{
		auto&& sopParms = cookparms.parms<SOP_HelloHDKParms>();
		auto sopCache = (SOP_HelloHDKCache*)cookparms.cache();

		// The output detail
		GEO_Detail* const output = cookparms.gdh().gdpNC();

		auto offset = output->appendPointBlock(sopParms.getSegmentCount() + 1);
		// Note:

		GA_RWHandleF position(output->findFloatTuple(GA_ATTRIB_POINT, "P"));
		if (!position.isValid())
		{
			position.bind(output->addFloatTuple(GA_ATTRIB_POINT, "P", 3));
		}
		if (!position.isValid())
		{
			cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, "P");
			return;
		}
	}

	void newSopOperator(OP_OperatorTable* table)
	{
		table->addOperator(new OP_Operator(
			SOP_HelloHDKVerb::theSOPTypeName,
			"Hello HDK",
			SOP_HelloHDK::myConstructor,
			SOP_HelloHDK::buildTemplates(),
			0,
			1,
			nullptr,
			0
		));
	}
