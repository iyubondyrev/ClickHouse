#include <Functions/IFunction.h>
#include <Functions/FunctionFactory.h>
#include <DataTypes/DataTypeString.h>
#include <Core/Field.h>


namespace DB
{
namespace
{

/// Returns name of IColumn instance.
class FunctionToColumnTypeName : public IFunction
{
public:
    static constexpr auto name = "toColumnTypeName";
    static FunctionPtr create(ContextPtr)
    {
        return std::make_shared<FunctionToColumnTypeName>();
    }

    String getName() const override
    {
        return name;
    }

    bool useDefaultImplementationForNulls() const override { return false; }

    bool useDefaultImplementationForNothing() const override { return false; }

    bool isShortCircuit(ShortCircuitSettings & settings, size_t /*number_of_arguments*/) const override
    {
        settings.enable_lazy_execution_for_first_argument = true;
        settings.enable_lazy_execution_for_common_descendants_of_arguments = true;
        settings.force_enable_lazy_execution = true;
        return true;
    }

    bool isSuitableForShortCircuitArgumentsExecution(const DataTypesWithConstInfo & /*arguments*/) const override { return false; }

    size_t getNumberOfArguments() const override
    {
        return 1;
    }

    DataTypePtr getReturnTypeImpl(const DataTypes & /*arguments*/) const override
    {
        return std::make_shared<DataTypeString>();
    }

    ColumnPtr executeImpl(const ColumnsWithTypeAndName & arguments, const DataTypePtr &, size_t input_rows_count) const override
    {
        return DataTypeString().createColumnConst(input_rows_count, arguments[0].column->getName());
    }

    ColumnPtr getConstantResultForNonConstArguments(const ColumnsWithTypeAndName & arguments, const DataTypePtr &) const override
    {
        return DataTypeString().createColumnConst(1, arguments[0].type->createColumn()->getName());
    }
};

}

void registerFunctionToColumnTypeName(FunctionFactory & factory)
{
    factory.registerFunction<FunctionToColumnTypeName>();
}

}
