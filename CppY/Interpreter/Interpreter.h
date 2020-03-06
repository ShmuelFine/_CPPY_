#pragma once

#include <string>
#include <vector>
#include <memory>
#include <regex>
#include "..\Base\Types\pyList.h"

namespace py
{
	class ICommand;
	typedef std::shared_ptr<ICommand> ICommandPtr;
	
	///////////////// ///////////////// /////////////////
	class PyLineParser
	{
	public:
		std::vector<ICommandPtr> ParsingChain;
		PyLineParser();

		ICommandPtr Parse(std::string const& line);
	};
	typedef std::shared_ptr<PyLineParser> PyLineParserPtr;

	///////////////// ///////////////// /////////////////
	
	class ICommandPostProcessor
	{
	public:
		virtual ICommandPtr Process(ICommandPtr in) = 0;
	};
	typedef std::shared_ptr<ICommandPostProcessor> ICommandPostProcessorPtr;

	///////////////// ///////////////// /////////////////

	class DefaultPostProcessor : public ICommandPostProcessor
	{
	public:
		ICommandPtr Process(ICommandPtr in) { return in; }
	};

	///////////////// ///////////////// /////////////////

	
	class ICommand
	{
	public:
		PyLineParser * _parser;
		
	public:
		ICommand(PyLineParser * parser) : _parser(parser) {}

		void ParsePy(std::string const & line);
		std::string Translate();
		std::set<std::string> GetVariables() const;

	public:
		virtual bool CanParse(std::string const& line) const = 0;
		virtual void ParsePy_inner(std::string const & line) = 0;
		virtual std::string Translate_inner() const = 0;
		virtual ICommandPtr Clone() const = 0;
	
		virtual bool IsToSemicolon() const { return true; }
		virtual std::string ScopeEndHook() const { return ""; }
		virtual ICommandPostProcessorPtr ScopeInnerHook() { return std::make_shared<DefaultPostProcessor>(); }

	protected:
		// default impl. for almost all commands:
		virtual std::vector<std::string> GetMyVariables() const {return std::vector<std::string>();}
	
	public:
		std::string Indentation;
		std::string CmdText;
		std::string SuffixSpace;

		std::vector<ICommandPtr> Children;
	};


	///////////////// ///////////////// /////////////////

#define COMMAND_INTERFACE(name, base)\
	class name : public base {\
	public:\
		name(PyLineParser * parser) : base(parser) {}\


#define COMMAND_CLASS(name, base)\
	COMMAND_INTERFACE(name, base)\
		virtual ICommandPtr Clone() const { \
	auto result = std::make_shared<name>(_parser); *result = *this; return result; }\

	
	COMMAND_INTERFACE(RegexBasedCommand, ICommand)
	public:
		virtual std::string GetRegexString() const = 0;
		virtual bool CanParse(std::string const& line) const override;
		virtual void ParsePy_inner(std::string const& line);
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch & matches) = 0;
	};

	/////////////////////////////////
#pragma region two sides operators
	COMMAND_INTERFACE(TwoSidesOperator,ICommand)
	public:
		virtual bool CanParse(std::string const& line) const override;
		virtual void ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;

		virtual std::string PyOperator() const = 0;
		virtual std::string CppOperator() const = 0;
	};

	COMMAND_CLASS(Assignment,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "="; }
		virtual std::string CppOperator() const override { return "="; }
		virtual std::vector<std::string> GetMyVariables() const;

	};

	COMMAND_CLASS(CommandLineSplitter, TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\;"; }
		virtual std::string CppOperator() const override { return ";"; }
	};

	COMMAND_CLASS(Addition, TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\+"; }
		virtual std::string CppOperator() const override { return "+"; }
	};

	COMMAND_CLASS(Substraction, TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\-"; }
		virtual std::string CppOperator() const override { return "-"; }
	};

	COMMAND_CLASS(Division, TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\/"; }
		virtual std::string CppOperator() const override { return "/"; }
	};
	
	COMMAND_CLASS(Multiplication, TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\*"; }
		virtual std::string CppOperator() const override { return "*"; }
	};

	COMMAND_CLASS(Bool_AND_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "\\sand\\s"; }
		virtual std::string CppOperator() const override { return " && "; }
	};

	COMMAND_CLASS(Bool_OR_Statement,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\sor\\s"; }
		virtual std::string CppOperator() const override { return " || "; }
	};

	COMMAND_CLASS(Bitwise_AND_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "\\&"; }
		virtual std::string CppOperator() const override { return "&"; }
	};

	COMMAND_CLASS(Bitwise_OR_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "\\|"; }
		virtual std::string CppOperator() const override { return "|"; }
	};

	///////// Comparisons: ////////////
	COMMAND_CLASS(LT,TwoSidesOperator)
	public: 
		virtual std::string PyOperator() const override { return "<"; }
		virtual std::string CppOperator() const override { return "<"; }
	};
	COMMAND_CLASS(LTE,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "<="; }
		virtual std::string CppOperator() const override { return "<="; }
	};
	COMMAND_CLASS(GT,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return ">"; }
		virtual std::string CppOperator() const override { return ">"; }
	};
	COMMAND_CLASS(GTE,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return ">="; }
		virtual std::string CppOperator() const override { return ">="; }
	};
	COMMAND_CLASS(Equal,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "=="; }
		virtual std::string CppOperator() const override { return "=="; }
	};
	COMMAND_CLASS(NotEqual,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "!="; }
		virtual std::string CppOperator() const override { return "!="; }
	};
	COMMAND_CLASS(ObjIdentity,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\sis\\s"; }
		virtual std::string CppOperator() const override;
		virtual std::string Translate_inner() const override;

	};
	COMMAND_CLASS(NgatedObjIdentity,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "\\sis not\\s"; }
		virtual std::string CppOperator() const override;
		virtual std::string Translate_inner() const override;

	};

	COMMAND_CLASS(NegationOP,RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;

	};
#pragma endregion

#pragma region containers : outer and inner
	//////////////////////////////////
	COMMAND_INTERFACE(InnerScopeEscaperBase,RegexBasedCommand)
	public:
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		
		virtual void ParsePy_innerScope(std::string const& text);
		virtual std::string GetRegexString() const override {
			// Enforce inner expression by the .+ at the beginning:
			return ".+(" + ScopeOpenerRGX() + ".*?" + ScopeCloserRGX() + ").*";
		}

		virtual std::string ScopeOpenerRGX() const = 0;
		virtual std::string ScopeCloserRGX() const = 0;

	public:
		std::string _UUID;
	};

	COMMAND_CLASS(InnerScope_CurlyBraces_Escaper,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\{)"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\})"; };
	};
	
	COMMAND_CLASS(InnerScope_SquareBraces_Escaper,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\[)"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\])"; };
	};

	COMMAND_CLASS(InnerScope_FunctionCall_Escaper,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\W\w+\()"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\))"; };
	};

	COMMAND_CLASS(InnerScope_RoundBraces_Escaper,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\()"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\))"; };
	};

	/////////////////////////////////

	COMMAND_INTERFACE(Container_ImplicitConstruction_Base,RegexBasedCommand)
	public:
		virtual std::string ScopeOpenerRGX() const = 0;
		virtual std::string ScopeCloserRGX() const = 0;
		// Inherited via ICommand
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual void ProcessMatch(std::string const& match);
		pyList elements;
		std::string _scopeOpener;
		std::string _scopeCloser;
	};

	COMMAND_CLASS(ListImplicitConstruction,Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\[)"; }
		virtual std::string ScopeCloserRGX() const override { return R"(\])"; }

		// Inherited via ICommand
		virtual std::string Translate_inner() const override;
	};

	COMMAND_CLASS(DictImplicitConstruction,Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\{.*?\:)"; }
		virtual std::string ScopeCloserRGX() const override { return R"(\})"; }

		virtual std::string Translate_inner() const override;
	};

	COMMAND_CLASS(EmptyDictImplicitConstruction,RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
	};


	COMMAND_CLASS(FunctionCall, Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\w+\()"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\))"; };

		virtual std::string Translate_inner() const override;
	};

	// Dict has precendence over set, this way we ensure we interpret it correctly.
	// (The regex does not enforce it)
	COMMAND_CLASS(SetImplicitConstruction,Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\{)"; }
		virtual std::string ScopeCloserRGX() const override { return R"(\})"; }

		// Inherited via ICommand
		virtual std::string Translate_inner() const override;
	};

	COMMAND_CLASS(BytesImplicitConstruction,Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(b\')"; }
		virtual std::string ScopeCloserRGX() const override { return R"(\')"; }

		// Inherited via ICommand
		virtual std::string Translate_inner() const override;
	};


#pragma endregion

	
#pragma region string literals and comments : outer and inner
	//////// STRING LITERALS: Inner /////////////////////

	COMMAND_CLASS(CommentLine_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string GetRegexString() const override {
			return R"(.*?(\#.*))";
		}
		virtual std::string ScopeOpenerRGX() const { return ""; }
		virtual std::string ScopeCloserRGX() const { return ""; }
	};
	
	COMMAND_INTERFACE(StringLiteral_InnerScope_Base, InnerScopeEscaperBase)
		// This class is for convinence of parsing.
	};

	COMMAND_CLASS(TripleQuote_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(''')"; }
		virtual std::string ScopeCloserRGX() const { return R"(''')"; }
	};

	COMMAND_CLASS(TripleDoubleQuote_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(""")"; }
		virtual std::string ScopeCloserRGX() const { return R"(""")"; }
	};

	COMMAND_CLASS(SingleQuote_REAL_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
	};

	COMMAND_CLASS(SingleQuote_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
	};

	COMMAND_CLASS(DoubleQuote_REAL_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r")"; }
		virtual std::string ScopeCloserRGX() const { return R"(")"; }
	};

	COMMAND_CLASS(DoubleQuote_StringLiteral_InnerScope,StringLiteral_InnerScope_Base)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(")"; }
		virtual std::string ScopeCloserRGX() const { return R"(")"; }
	};

	//////// STRING LITERALS: Outer /////////////////////
	COMMAND_INTERFACE(StringLiterals_OuterScope_EscaperBase,Container_ImplicitConstruction_Base)
	public:

		virtual std::string ScopeOpenerRGX() const = 0;
		virtual std::string ScopeCloserRGX() const = 0;

		virtual void ProcessMatch(std::string const& match);

		virtual bool IsRealString() const = 0;
		
		virtual std::string Translate_inner() const;
	};

	COMMAND_CLASS(TripleQuote_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(''')"; }
		virtual std::string ScopeCloserRGX() const { return R"(''')"; }
		virtual bool IsRealString() const { return true; }
	};

	COMMAND_CLASS(TripleDoubleQuote_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(""")"; }
		virtual std::string ScopeCloserRGX() const { return R"(""")"; }
		virtual bool IsRealString() const { return true; }
	};

	COMMAND_CLASS(SingleQuote_REAL_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
		virtual bool IsRealString() const { return true; }
	};

	COMMAND_CLASS(SingleQuote_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
		virtual bool IsRealString() const { return false; }
	};

	COMMAND_CLASS(DoubleQuote_REAL_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r")"; }
		virtual std::string ScopeCloserRGX() const { return R"(")"; }
		virtual bool IsRealString() const { return true; }
	};

	COMMAND_CLASS(DoubleQuote_StringLiteral_OuterScope,StringLiterals_OuterScope_EscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(")"; }
		virtual std::string ScopeCloserRGX() const { return R"(")"; }
		virtual bool IsRealString() const { return false; }
	};

#pragma endregion

	///////////////////////////////////
	COMMAND_CLASS(If_Statement,RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }

	};
	
	COMMAND_CLASS(Else_Statement, RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }
	};

	COMMAND_CLASS(For_Statement, RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual std::vector<std::string> GetMyVariables() const;
		virtual bool IsToSemicolon() const { return false; }

	};

	COMMAND_CLASS(While_Statement,RegexBasedCommand)	
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }
	};


	////////////////////////////////
	COMMAND_CLASS(SameLine, ICommand)
	public:
		std::string Line;
		virtual bool CanParse(std::string const& line) const override;
		virtual void ParsePy_inner(std::string const & line) override;
		virtual std::string Translate_inner() const override;
	};

	COMMAND_CLASS(EmptyLine, SameLine)
	public:
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }
	};

	ICommandPtr GetEmptyCommand();
	//////////////////////////////////
	// FUN DEFS						//
	//////////////////////////////////
	class FunDef_Scope_PostProcessor : public ICommandPostProcessor
	{
	public:
		ICommandPtr docString_ptr;
		bool IsBeforeFirstMeaningfullLine;
		FunDef_Scope_PostProcessor();

	public:
		virtual ICommandPtr Process(ICommandPtr in);
	};
	COMMAND_CLASS(FunDef, RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }
		virtual ICommandPostProcessorPtr ScopeInnerHook() override;
		virtual std::string ScopeEndHook() const;
	public:
		std::string Name;
		std::vector<std::pair<std::string, std::string>> Params;
		std::shared_ptr<FunDef_Scope_PostProcessor> InnerScopeProcessor;
	};

	//////////////////////////////////
	// CLASS DEFS						//
	//////////////////////////////////
	class ClassDef_Scope_PostProcessor : public ICommandPostProcessor
	{
	public:
		std::vector<std::string> FuncNames;
		ICommandPtr docString_ptr;
		bool IsBeforeFirstMeaningfullLine;
		ClassDef_Scope_PostProcessor();

	public:
		virtual ICommandPtr Process(ICommandPtr in);
	};
	COMMAND_CLASS(ClassDef, RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
		virtual bool IsToSemicolon() const { return false; }
		virtual ICommandPostProcessorPtr ScopeInnerHook() override;
		virtual std::string ScopeEndHook() const;
	public:
		std::string ClassName;
		std::string ParentName;
		std::shared_ptr<ClassDef_Scope_PostProcessor> InnerScopeProcessor;
	};


	
	COMMAND_CLASS(MemberInvocation, RegexBasedCommand)
	public:
		virtual std::string GetRegexString() const override;
		virtual void ParsePy_inner_byRegex(std::string const& line, std::smatch& matches) override;
		virtual std::string Translate_inner() const override;
	public:
		std::string VarName;
		std::string MemberName;
	};
	
}

