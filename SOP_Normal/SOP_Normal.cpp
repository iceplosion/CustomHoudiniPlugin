
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

class PRM_Template;

class SOP_Normal : public SOP_Node
{
public:
    static PRM_Template *buildTemplates();
    static OP_Node *myConstructor(OP_Network *net, const char *name, OP_Operator *op)
    {
        return new SOP_Normal(net, name, op);
    }
    const SOP_NodeVerb* cookVerb() const override;
protected:
    SOP_Normal(OP_Network* net, const char *name, OP_Operator *op) 
        : SOP_Node(net, name, op)
    {
        mySopFlags.setManagesDataIDs(true);
    }
    ~SOP_Normal() override {}

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

	class SOP_NormalParms : public SOP_NodeParms
	{
	public:
		static int version() { return 1; }
		SOP_NormalParms()
		{}

		explicit SOP_NormalParms(const SOP_NormalParms&) = default;

		~SOP_NormalParms() override {}

		bool operator==(const SOP_NormalParms& src) const
		{
			return true;
		}

		bool operator!=(const SOP_NormalParms& src) const
		{
			return !operator==(src);
		}

		void buildFromOp(const SOP_GraphProxy* graph, exint nodeidx, fpreal time, DEP_MicroNode* depnode)
		{
		}

		void loadFromOpSubclass(const LoadParms& loadparms) override
		{
			buildFromOp(loadparms.graph(), loadparms.nodeIdx(), loadparms.context().getTime(), loadparms.depnode());
		}

		void copyFrom(const SOP_NodeParms* src) override
		{
			*this = *((const SOP_NormalParms*)src);
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
			return 0;
		}

		ParmType getNestParmType(TempIndex fieldnum) const override
		{
			if (fieldnum.size() < 1)
				return PARM_UNSUPPORTED;
			return PARM_UNSUPPORTED;
		}
	};

	class SOP_NormalCache : public SOP_NodeCache
	{
	public:
		SOP_NormalCache() : SOP_NodeCache()
		{}
		virtual ~SOP_NormalCache() {}
	};

	class SOP_NormalVerb : public SOP_NodeVerb
	{
	public:
		static const UT_StringHolder theSOPTypeName;
		static const SOP_NodeVerb::Register<SOP_NormalVerb> theVerb;
		static const char* const theDsFile;

		virtual SOP_NodeParms* allocParms() const { return new SOP_NormalParms(); }
		virtual SOP_NodeCache* allocCache() const { return new SOP_NormalCache(); }
		virtual UT_StringHolder name() const { return theSOPTypeName; }

		virtual CookMode cookMode(const SOP_NodeParms* parms) const { return COOK_INPLACE; }
		virtual void cook(const CookParms& cookParms) const;
	};

	const UT_StringHolder SOP_NormalVerb::theSOPTypeName("SOP_Normal"_sh);
	const SOP_NodeVerb::Register<SOP_NormalVerb> SOP_NormalVerb::theVerb;

	PRM_Template* SOP_Normal::buildTemplates()
	{
		static PRM_TemplateBuilder templ("SOP_Normal.cpp"_sh, SOP_NormalVerb::theDsFile);
        if (templ.justBuilt())
        {

        }
		return templ.templates();
	}

	const SOP_NodeVerb* SOP_Normal::cookVerb() const
	{
		return SOP_NormalVerb::theVerb.get();
	}

	const char *const SOP_NormalVerb::theDsFile = R"THEDSFILE(
	{
		name parameters
		parm {

		}
	})THEDSFILE";

	void SOP_NormalVerb::cook(const CookParms& cookparms) const
	{
		auto&& sopParms = cookparms.parms<SOP_NormalParms>();
		auto sopCache = (SOP_NormalCache*)cookparms.cache();
		// The output detail
		GEO_Detail* const output = cookparms.gdh().gdpNC();
		GA_RWHandleV3 normalAttr(output->findFloatTuple(GA_ATTRIB_PRIMITIVE, "Normal"));
		if (!normalAttr.isValid())
		{
			normalAttr.bind(output->addFloatTuple(GA_ATTRIB_PRIMITIVE, "Normal", 3));
		}
		if (!normalAttr.isValid())
		{
			cookparms.sopAddError(SOP_ATTRIBUTE_INVALID, "Normal");
		}
	}

	void newSopOperator(OP_OperatorTable* table)
	{
		table->addOperator(new OP_Operator(
			SOP_NormalVerb::theSOPTypeName,
			"Normal",
			SOP_Normal::myConstructor,
			SOP_Normal::buildTemplates(),
			0,
			1,
			nullptr,
			0
		));
	}
