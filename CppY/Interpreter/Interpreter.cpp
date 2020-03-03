#include "Interpreter.h"
#include "object.h"
#include "list.h"
#include "Comparisons.h"
#include <string>
#include "_Exceptions.h"

namespace py
{

	void ICommand::ParsePy(std::string const & line)
	{
		std::regex statement(R"(^(\s*)(.+?)(\s*)$)");
		if (!std::regex_match(line, statement))
		{
			Indentation = "";
			CmdText = "";
			SuffixSpace = "";
			return;
		}

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Indentation = matches[1];
		CmdText = matches[2];
		SuffixSpace = matches[3];

		ParsePy_inner(CmdText);
	}

	std::string ICommand::Translate()
	{
		return Indentation + Translate_inner() + SuffixSpace;
	}
	
	std::set<std::string> ICommand::GetVariables() const
	{
		auto myVars = GetMyVariables();
		std::set<std::string> result(myVars.begin(), myVars.end());
		for (auto& child : Children)
		{
			auto toAdd = child->GetVariables();
			if (!toAdd.empty())
				result.insert(toAdd.begin(), toAdd.end());
		}
		return result;
	}

	//////////////////////////////////////////////////

	bool RegexBasedCommand::CanParse(std::string const& line) const
	{
		std::regex statement(GetRegexString());
		std::string justText = pyStr(line).strip();
		return std::regex_match(justText, statement);
	}
	void RegexBasedCommand::ParsePy_inner(std::string const& line)
	{
		std::regex statement(GetRegexString());
		std::smatch matches;
		std::regex_search(line, matches, statement);
		ParsePy_inner_byRegex(line, matches);
	}
	//////////////////////////////////////////////////
	std::string GetVarialbeRegex()
	{
		return R"(^([a-zA-Z_]+[a-zA-Z0-9_]*)$)";
	}
	bool IsVarialbe(std::string const& line)
	{
		std::regex varName(GetVarialbeRegex());
		return std::regex_match(line, varName);
	}
	std::vector<std::string> ExtractVariables(std::string const& candidatesStr)
	{
		std::vector<std::string> result;
		auto varCandidates = pyStr(candidatesStr).split(R"(\,)");
		for (auto candidate : varCandidates)
		{
			if (IsVarialbe(candidate))
				result.push_back(candidate);
		}
		return result;
	}
	
	//////////////////////////////////////////////////

	bool SameLine::CanParse(std::string const& line) const
	{
		return true;
	}

	void SameLine::ParsePy_inner(std::string const &  line)
	{
		Line = line;
	}
	std::string SameLine::Translate_inner() const
	{
		return Line;
	}
	
	std::string EmptyLine::Translate_inner() const
	{
		return "";
	}

	ICommandPtr GetEmptyCommand()
	{
		return std::make_shared<EmptyLine>(nullptr);
	}

	bool TwoSidesOperator::CanParse(std::string const& line) const
	{
		return pyStr(line).split(PyOperator(), 2).size() > 1;
	}

	void TwoSidesOperator::ParsePy_inner(std::string const &  line)
	{
		auto parts = pyStr(line).split(PyOperator(), 2);
		if (parts.size() > 1)
		{
			if (parts[0] == "")
				__debugbreak();

			Children.push_back(_parser->Parse(pyStr(parts[0])));
			Children.push_back(_parser->Parse(pyStr(parts[1])));
		}
	}

	std::string TwoSidesOperator::Translate_inner() const
	{
		return Children[0]->Translate() + CppOperator() + Children[1]->Translate();
	}
	
	//////////////////////////////////////////////
	std::vector<std::string> Assignment::GetMyVariables() const
	{
		return ExtractVariables(Children[0]->Translate_inner());
	}
	//////////////////////////////////////////////

	std::string ObjIdentity::CppOperator() const { NOT_IMPL; }
	std::string ObjIdentity::Translate_inner() const
	{
		return pyStr("is_same(") + Children[0]->Translate() + pyStr(",") + Children[1]->Translate() + pyStr(")");
	}

	std::string NgatedObjIdentity::CppOperator() const { NOT_IMPL; }
	std::string NgatedObjIdentity::Translate_inner() const
	{
		return pyStr("is_not(") + Children[0]->Translate() + pyStr(",") + Children[1]->Translate() + pyStr(")");
	}
	   
	std::string NegationOP::GetRegexString() const
	{
		return R"(not (.*))";
	}
	void NegationOP::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		Children.push_back(_parser->Parse(matches[1]));
	}

	std::string NegationOP::Translate_inner() const
	{
		return "! " + Children[0]->Translate();
	}

	//////////// Pre and post escaping: ///////////////
	std::string UUID()
	{
		std::string s(16, 0);
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		for (int i = 0; i < s.size(); ++i) {
			s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		return "$"+s+"$";
	}

	
	///////////////////////////
	
	void InnerScopeEscaperBase::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		std::string innerExpression_str = matches[1];

		_UUID = UUID();
		std::string modifiedLine =
			std::string(line.begin(), line.begin() + matches.position(1))
			+ _UUID +
			std::string(line.begin() + matches.position(1) + matches.length(1), line.end());

		Children.push_back(_parser->Parse(modifiedLine));
		ParsePy_innerScope(innerExpression_str);
	}
	
	std::string InnerScopeEscaperBase::Translate_inner() const
	{
		std::string outer = Children[0]->Translate();
		std::string inner = Children[1]->Translate();
		
		auto uuidIndex = outer.find(_UUID, 0);
		THROW_IF(std::string::npos == uuidIndex, "Parsing error: Where did the substring UUID go?!");
		outer.replace(uuidIndex, _UUID.size(), inner);

		return outer;
	}

	void InnerScopeEscaperBase::ParsePy_innerScope(std::string const& text) { Children.push_back(_parser->Parse(text)); }

	
	////////////////////////////////////////////////////////////

	std::string Container_ImplicitConstruction_Base::GetRegexString() const
	{
		return "^(" + ScopeOpenerRGX() + ")" + R"((.*))" + "(" + ScopeCloserRGX() + ")" + "$";
	}

	void Container_ImplicitConstruction_Base::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		_scopeOpener = matches[1];
		std::string elements = matches[2];
		_scopeCloser = matches[3];
		ProcessMatch(elements);
	}

	void Container_ImplicitConstruction_Base::ProcessMatch(std::string const& match)
	{
		for (pyStr part : pyStr(match).split(","))
			elements.append(part.Clone());
	}

	std::string ListImplicitConstruction::Translate_inner() const
	{
		return pyStr("list({") + pyStr(",").join(elements) + pyStr("})");
	}

	std::string DictImplicitConstruction::Translate_inner() const
	{
		pyStr result = pyStr("dict({");
		for (auto elemObj : elements)
		{
			pyStr elem = (std::string)*elemObj;
			auto parts = elem.split(":");
			THROW_UNLESS(parts.size() == 2, "Can't construct dict value from " + std::to_string(parts.size()) + " parts");
			result += pyStr("{ {} , {} } , ").format(parts[0], parts[1]);
		}
		result += pyStr("})");
		return result;
	}

	std::string FunctionCall::Translate_inner() const
	{
		pyStr result = pyStr("");

		for (auto elemObj : elements)
		{
			pyStr elem = (std::string) * elemObj;
			auto parts = elem.split("=");
			for (auto& p : parts)
				p = pyStr(p).strip();

			THROW_UNLESS(parts.size() <= 2, "Can't construct dict value from " + std::to_string(parts.size()) + " parts");
			result += result ? pyStr(".A") : pyStr("ARGS");
			if (parts.size() == 2)
				result += pyStr("({} , {})").format(parts[0], parts[1]);
			else // single part:
				result += pyStr("({})").format(parts[0]);
		}
		
		return pyStr(_scopeOpener) + result  + pyStr(_scopeCloser);
	}


	std::string EmptyDictImplicitConstruction::GetRegexString() const
	{
		return "\\{\\s*\\}";
	}

	void EmptyDictImplicitConstruction::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) 
	{}

	std::string EmptyDictImplicitConstruction::Translate_inner() const
	{
		return "dict()";
	}

	std::string SetImplicitConstruction::Translate_inner() const
	{
		return pyStr("set({") + pyStr(",").join(elements) + pyStr("})");
	}

	std::string BytesImplicitConstruction::Translate_inner() const
	{
		return pyStr("bytes(\"") + pyStr(",").join(elements) + pyStr("\")");
	}

	////////////////////////////////////////////////////



	std::string If_Statement::GetRegexString() const
	{
		return R"(if\s+(.+)\:)";
	}
	void If_Statement::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		Children.push_back(_parser->Parse(matches[1]));
	}

	std::string If_Statement::Translate_inner() const
	{
		return pyStr("if ({})").format(Children[0]->Translate());
	}

	
	std::string For_Statement::GetRegexString() const
	{
		return R"(for\s+(.+)\sin\s+(.+)\:)";
	}
	
	void For_Statement::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		Children.push_back(_parser->Parse(matches[1]));
		Children.push_back(_parser->Parse(matches[2]));
	}
	std::string For_Statement::Translate_inner() const
	{
		return pyStr("for (auto {} : {})").format(Children[0]->Translate(), Children[1]->Translate());
	}
	std::vector<std::string> For_Statement::GetMyVariables() const
	{
		return ExtractVariables(Children[0]->Translate_inner());
	}


	std::string While_Statement::GetRegexString() const
	{
		return R"(while\s+(.+)\:)";
	}

	void While_Statement::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		Children.push_back(_parser->Parse(matches[1]));
	}
	std::string While_Statement::Translate_inner() const
	{
		return pyStr("while ({})").format(Children[0]->Translate());
	}

	//void InnerScope_FunctionCall_Escaper::ParsePy_innerScope(std::string const& text)
	//{
	//	// Surround with $ which is not used in python (https://docs.python.org/3/reference/lexical_analysis.html)
	//	// for invoking fucn. - ARGs creation:
	//	Children.push_back(_parser->Parse("$" + text + "$"));
	//}

	void StringLiterals_OuterScope_EscaperBase::ProcessMatch(std::string const& match)
	{
		elements.append(pyStr(match).Clone());
	}

	std::string StringLiterals_OuterScope_EscaperBase::Translate_inner() const
	{
		pyStr result;
		if (IsRealString())
			result += pyStr(R"(R"()");
		else
			result += pyStr(R"(")");

		result += elements ? (pyStr)*elements.FetchByIdx(0) : pyStr("") ;

		if (IsRealString())
			result += pyStr(")\"");
		else
			result += pyStr(R"(")");

		return result;
	}

	/////////////////

	std::string Else_Statement::GetRegexString() const { return "else\\:"; }
	void Else_Statement::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) {}
	std::string Else_Statement::Translate_inner() const { return "else"; }

	///////////////// ///////////////// ///////////////// ///////////////// ///////////////// /////////////////

	FunDef_Scope_PostProcessor::FunDef_Scope_PostProcessor() : docString_ptr(nullptr) {}

	ICommandPtr FunDef_Scope_PostProcessor::Process(ICommandPtr in)
	{
		if (docString_ptr)
			return in;

		auto strippedContent = pyStr(in->Translate()).strip();
		bool isJustSpacesWithNoContent = ! strippedContent;
		if (isJustSpacesWithNoContent)
			return in;

		// here it is. The first line which isn't empty.
		// Either it's a doc string or we don't have a doc string;

		auto StringLiteral_cmd_Ptr = std::reinterpret_pointer_cast<StringLiterals_OuterScope_EscaperBase>(in);
		if (StringLiteral_cmd_Ptr)
		{
			docString_ptr = in;
			return GetEmptyCommand();
		}
		else
		{
			docString_ptr = GetEmptyCommand();
			return in;
		}
	}

	///////////////// ///////////////// ///////////////// ///////////////// ///////////////// /////////////////
	std::string FunDef::GetRegexString() const
	{
		return R"(def\s+(\w+)\s*\((.*?)\)\:)";
	}

	void FunDef::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		Name = matches[1];
		pyStr allParams_str = matches[2].str();
		auto params = allParams_str.split(",");
		for (pyStr param : params)
		{
			auto parts = param.split("=");
			for (auto& p : parts)
				p = pyStr(p).strip();

			if (parts.size() == 1)
				parts.push_back("");

			Params.push_back(std::make_pair(parts[0], parts[1]));
		}
	}

	std::string FunDef::Translate_inner() const
	{
		auto result = "FUN_DEF_WITH_DOC(" + Name + ", " + "DocString" + ");";
		for (auto paramPair : Params)
		{
			result += "\nPARAM(" + paramPair.first + "," + paramPair.second + ");";
		}
		return result;
	}

	ICommandPostProcessorPtr FunDef::ScopeInnerHook()
	{
		InnerScopeProcessor.reset(new FunDef_Scope_PostProcessor());
		return InnerScopeProcessor;
	}


	std::string FunDef::ScopeEndHook() const
	{		
		return "END_FUN_WITH_DOC_STR(" + Name + ", " + InnerScopeProcessor->docString_ptr->Translate() + ");";
	}


	///////////////// ///////////////// ///////////////// ///////////////// ///////////////// /////////////////

	ClassDef_Scope_PostProcessor::ClassDef_Scope_PostProcessor() : FuncNames() {}

	ICommandPtr ClassDef_Scope_PostProcessor::Process(ICommandPtr in)
	{
		auto FunDef_cmd_Ptr = std::reinterpret_pointer_cast<FunDef>(in);
		if (FunDef_cmd_Ptr)
		{
			FuncNames.push_back(FunDef_cmd_Ptr->Name);
		}

		return in;
	}

	///////////////// ///////////////// ///////////////// ///////////////// ///////////////// /////////////////

	std::string ClassDef::GetRegexString() const
	{
		return R"(class\s+(\w+)(\(\w+\))?\:)";
	}

	void ClassDef::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) 
	{
		ClassName = matches[1];
		ParentName = matches.size() > 2 ? matches[2].str() : "";
	}

	std::string ClassDef::Translate_inner() const 
	{ 
		auto result = "class " + ClassName;
		if (ParentName.empty())
		{
			result += " : public object";
		}
		else
		{
			result += " : public " + ParentName;
		}
		result += 
R"(
{
public:
	)" + ClassName + R"(();
protected:
	void AddAttributes();
};)";
		return result;
	}

	ICommandPostProcessorPtr ClassDef::ScopeInnerHook()
	{
		InnerScopeProcessor.reset(new ClassDef_Scope_PostProcessor());
		return InnerScopeProcessor;
	}


	std::string ClassDef::ScopeEndHook() const
	{
		std::string ctor = 
ClassName + "::" + ClassName + "() : object(" + ParentName + "())" + R"(
{
	AddAttributes();
})";
		std::string addAttrs =
			"void " + ClassName + "::AddAttributes()" + "\n{";
		
		for (auto& name : InnerScopeProcessor->FuncNames)
			addAttrs += "\n\tattr(" + name + ") = " + name + ";";

		addAttrs += "\n}";
		return ctor + "\n\n" + addAttrs;
	}


	/////////////////

	std::string MemberInvocation::GetRegexString() const
	{
		return R"(^([a-zA-Z_]\w+)\.([a-zA-Z_]\w+))";
	}

	void MemberInvocation::ParsePy_inner_byRegex(std::string const& line, std::smatch& matches)
	{
		VarName = matches[1];
		MemberName = matches[2];
	}

	std::string MemberInvocation::Translate_inner() const
	{
		return VarName + ".attr(" + MemberName + ")";
	}
	/////////////////


	PyLineParser::PyLineParser()
	{
		ParsingChain.push_back(std::make_shared<CommentLine_InnerScope							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<FunDef											>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<ClassDef										>((PyLineParser*)this));


		ParsingChain.push_back(std::make_shared<InnerScope_CurlyBraces_Escaper 					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_SquareBraces_Escaper					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_FunctionCall_Escaper					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<FunctionCall									>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<InnerScope_RoundBraces_Escaper 					>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<ListImplicitConstruction						>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DictImplicitConstruction						>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<EmptyDictImplicitConstruction					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SetImplicitConstruction							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<BytesImplicitConstruction						>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<If_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Else_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<For_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<While_Statement			  						>((PyLineParser*)this));


		ParsingChain.push_back(std::make_shared<Assignment										>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_AND_Statement								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_OR_Statement								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_AND_Statement							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_OR_Statement							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Addition										>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Substraction									>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Division										>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Multiplication									>((PyLineParser*)this));

		ParsingChain.push_back(std::make_shared<LT				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<LTE				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<GT				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<GTE				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Equal			  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NotEqual		  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<ObjIdentity		  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NgatedObjIdentity 								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NegationOP		  								>((PyLineParser *)this));

		
		ParsingChain.push_back(std::make_shared<SameLine				  						>((PyLineParser *)this));
	}

	ICommandPtr PyLineParser::Parse(std::string const& origLine)
	{
		for (auto parserPtr : ParsingChain)
		{
			auto line = origLine;
			if (parserPtr->CanParse(line))
			{
				auto result = parserPtr->Clone();
				result->ParsePy(line);
				return result;
			}
		}

		throw std::runtime_error("Shouldn't get here... What happened to the SameLine parser?");
	}
}


