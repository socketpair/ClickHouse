#include <Functions/IFunctionImpl.h>
#include <Functions/FunctionFactory.h>
#include <DataTypes/DataTypeString.h>
#include <common/getFQDNOrHostName.h>
#include <Core/Field.h>


namespace DB
{

class FunctionFQDN : public IFunction
{
public:
    static constexpr auto name = "FQDN";
    static FunctionPtr create(const Context &)
    {
        return std::make_shared<FunctionFQDN>();
    }

    String getName() const override
    {
        return name;
    }

    bool isDeterministic() const override { return false; }

    size_t getNumberOfArguments() const override
    {
        return 0;
    }

    DataTypePtr getReturnTypeImpl(const DataTypes & /*arguments*/) const override
    {
        return std::make_shared<DataTypeString>();
    }

    void executeImpl(ColumnsWithTypeAndName & columns, const ColumnNumbers &, size_t result, size_t input_rows_count) const override
    {
        columns[result].column = columns[result].type->createColumnConst(
            input_rows_count, getFQDNOrHostName())->convertToFullColumnIfConst();
    }
};


void registerFunctionFQDN(FunctionFactory & factory)
{
    factory.registerFunction<FunctionFQDN>(FunctionFactory::CaseInsensitive);
    factory.registerFunction<FunctionFQDN>("fullHostName");
}

}
