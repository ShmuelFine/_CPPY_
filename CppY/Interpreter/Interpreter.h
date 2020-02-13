#pragma once

#include <string>
#include <vector>
#include <memory>
#include "..\Base\Types\pyList.h"

namespace py
{
	class ICommand;
	typedef std::shared_ptr<ICommand> ICommandPtr;
	
	class VariablesMetaData;
	typedef std::shared_ptr<VariablesMetaData> VariablesMetaDataPtr;
	class VariablesMetaData
	{
	public:
		// Returns true if this name is a new one that wasn't previously defined.
		// Returns false otherwise.
		bool IsDefined(std::string const& varName)
		{
			bool isDefinedInMe = (0 < std::count(_currentScopeVars.begin(), _currentScopeVars.end(), varName));
			bool isDefinedInParent = _parentScopeMD && _parentScopeMD->IsDefined(varName);
			return (!(isDefinedInMe || isDefinedInParent));
		}

		void Add(std::string const& varName)
		{
			_currentScopeVars.push_back(varName);
		}

	protected:
		VariablesMetaDataPtr _parentScopeMD;
		std::vector<std::string> _currentScopeVars;
	};
	
	class PyParser
	{
	public:
		std::vector<ICommandPtr> ParsingChain;
		std::vector<VariablesMetaDataPtr> ScopeMDs_Stack;
		VariablesMetaDataPtr ScopeMD();
		std::vector<int> Indintations;

		PyParser();

		ICommandPtr Parse(std::string const& line);
	};
	typedef std::shared_ptr<PyParser> PyParserPtr;

	class ICommand
	{
	public:
		PyParser * _parser;
		
	public:
		ICommand(PyParser * parser) : _parser(parser) {}

		// Important! The input is always assumed to be stripped
		bool ParsePy(std::string const & line);
		std::string Translate();
	public:
		virtual bool ParsePy_inner(std::string const & line) = 0;
		virtual std::string Translate_inner() const = 0;
		
		virtual ICommandPtr Clone() const = 0;

		std::string Indentation;
		std::string CmdText;
		std::string SuffixSpace;
		std::vector<ICommandPtr> Children;
	};

#define COMMAND_INTERFACE(name, base)\
	class name : public base {\
	public:\
		name(PyParser * parser) : base(parser) {}\


#define COMMAND_CLASS(name, base)\
	COMMAND_INTERFACE(name, base)\
		virtual ICommandPtr Clone() const { \
	auto result = std::make_shared<name>(_parser); *result = *this; return result; }


	/////////////////////////////////
#pragma region two sides operators
	COMMAND_INTERFACE(TwoSidesOperator,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;

		virtual std::string PyOperator() const = 0;
		virtual std::string CppOperator() const = 0;
	};

	COMMAND_CLASS(Assignment,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "="; }
		virtual std::string CppOperator() const override { return "="; }
	};

	COMMAND_CLASS(Bool_AND_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "and"; }
		virtual std::string CppOperator() const override { return "&&"; }
	};

	COMMAND_CLASS(Bool_OR_Statement,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "or"; }
		virtual std::string CppOperator() const override { return "\\|\\|"; }
	};

	COMMAND_CLASS(Bitwise_AND_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "\\&"; }
		virtual std::string CppOperator() const override { return "\\&"; }
	};

	COMMAND_CLASS(Bitwise_OR_Statement,TwoSidesOperator)
	public:

		virtual std::string PyOperator() const override { return "\\|"; }
		virtual std::string CppOperator() const override { return "\\|"; }
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
		virtual std::string PyOperator() const override { return "is"; }
		virtual std::string CppOperator() const override;
		virtual std::string Translate_inner() const override;

	};
	COMMAND_CLASS(NgatedObjIdentity,TwoSidesOperator)
	public:
		virtual std::string PyOperator() const override { return "is not"; }
		virtual std::string CppOperator() const override;
		virtual std::string Translate_inner() const override;

	};

	COMMAND_CLASS(NegationOP,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;

	};
#pragma endregion

#pragma region containers : outer and inner
	//////////////////////////////////
	COMMAND_INTERFACE(InnerScopeEscaperBase,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;
		
		virtual void ParsePy_innerScope(std::string const& text);
		virtual std::string OuterExpressionRegex() {
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
		//virtual void ParsePy_innerScope(std::string const& text);
	};

	COMMAND_CLASS(InnerScope_RoundBraces_Escaper,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(\()"; };
		virtual std::string ScopeCloserRGX() const override { return R"(\))"; };
	};

	/////////////////////////////////

	COMMAND_INTERFACE(Container_ImplicitConstruction_Base,ICommand)
	public:
		virtual std::string ScopeOpenerRGX() const = 0;
		virtual std::string ScopeCloserRGX() const = 0;
		// Inherited via ICommand
		virtual bool ParsePy_inner(std::string const & line) override;
		virtual void ProcessMatch(std::string const& match);
		pyList elements;
		std::string _scopeOpener;
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

	COMMAND_CLASS(EmptyDictImplicitConstruction,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const & line) override;
		virtual std::string Translate_inner() const override;
	};


	COMMAND_CLASS(FunctionCall, Container_ImplicitConstruction_Base)
	public:
		virtual std::string ScopeOpenerRGX() const override { return R"(w+\()"; };
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

	
#pragma region string literals : outer and inner
	//////// STRING LITERALS: Inner /////////////////////

	COMMAND_CLASS(CommentLine_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string OuterExpressionRegex() override {
			// Enforce inner expression by the .+ at the beginning:
			return R"(.*?(\#.*))";
		}
		virtual std::string ScopeOpenerRGX() const { return ""; }
		virtual std::string ScopeCloserRGX() const { return ""; }
	};

	COMMAND_CLASS(TripleQuote_StringLiteral_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(''')"; }
		virtual std::string ScopeCloserRGX() const { return R"(''')"; }
	};

	COMMAND_CLASS(TripleDoubleQuote_StringLiteral_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(""")"; }
		virtual std::string ScopeCloserRGX() const { return R"(""")"; }
	};

	COMMAND_CLASS(SingleQuote_REAL_StringLiteral_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
	};

	COMMAND_CLASS(SingleQuote_StringLiteral_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(')"; }
		virtual std::string ScopeCloserRGX() const { return R"(')"; }
	};

	COMMAND_CLASS(DoubleQuote_REAL_StringLiteral_InnerScope,InnerScopeEscaperBase)
	public:
		virtual std::string ScopeOpenerRGX() const { return R"(r")"; }
		virtual std::string ScopeCloserRGX() const { return R"(")"; }
	};

	COMMAND_CLASS(DoubleQuote_StringLiteral_InnerScope,InnerScopeEscaperBase)
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

	/////////////////////////////////

	COMMAND_CLASS(VariableUsage,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;

		std::string VarName;
		bool IsDefinedFirstTime;
	};

	///////////////////////////////////
	COMMAND_CLASS(If_Statement,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;
	};
	
	COMMAND_CLASS(For_Statement,ICommand)
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;
	};

	COMMAND_CLASS(While_Statement,ICommand)	
	public:
		virtual bool ParsePy_inner(std::string const &  line) override;
		virtual std::string Translate_inner() const override;
	};


	////////////////////////////////
	COMMAND_CLASS(SameLine, ICommand)
	public:
		std::string Line;
		virtual bool ParsePy_inner(std::string const & line) override;
		virtual std::string Translate_inner() const override;
	};

}

