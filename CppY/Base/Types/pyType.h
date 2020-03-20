#pragma once
#include "pyObj.h"
#include "_Exceptions.h"

namespace py
{

	class pyType : public pyObj
	{
	public:
		pyObjPtr Template;

	public:
		pyType(pyObj const& typeTemplate)
			:Template(typeTemplate.Clone())
		{}

		////////////////// Inherited via pyObj ////////////////// 
		virtual operator std::string() const override { NOT_IMPL; }

		virtual operator double() const override	{NOT_IMPL;}
		virtual operator float() const override		{NOT_IMPL;}
		virtual operator int() const override		{NOT_IMPL;}
		virtual operator bool() const override		{NOT_IMPL;}
		virtual pyObjPtr& operator[](pyObjPtr const& key) override { NOT_IMPL; }
		virtual pyObjPtr FetchByIdx(int idx) const override { NOT_IMPL; }
		virtual pyObjIterator begin() const override { NOT_IMPL; }
		virtual pyObjIterator end() const override { NOT_IMPL; }
		virtual pyObjPtr operator()(pyObj const& params) const override { NOT_IMPL; }
		virtual std::string Type() const override { return "type"; }
		virtual pyObjPtr Clone() const override;
		virtual bool operator<(pyObj const& other) const override { NOT_IMPL; }
		virtual pyObjPtr operator++(int) override { NOT_IMPL; }
		virtual pyObj& operator++() override { NOT_IMPL; }
		virtual pyObj& operator+=(pyObj const& other) override { NOT_IMPL; }
	};

}