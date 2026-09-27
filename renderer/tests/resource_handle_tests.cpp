#include <gtest/gtest.h>
#include <resource_handle.h>

using namespace renderer;

namespace renderer::test
{
	class ResourceHandleTest : public ::testing::Test
	{
	protected:

	};

	TEST_F(ResourceHandleTest, DefaultKindIsInvalid)
	{
		ASSERT_EQ(ResourceHandle().GetKind(), ResourceKind::Invalid);
	}

	TEST_F(ResourceHandleTest, DefaultValueIsInvalid)
	{
		ASSERT_EQ(ResourceHandle().GetValue(), ResourceHandle::InvalidValue);
	}	

	TEST_F(ResourceHandleTest, AccessorsReturnExpected)
	{
		ResourceHandle handle(ResourceKind::Light, 42);
		ASSERT_EQ(handle.GetKind(), ResourceKind::Light);
		ASSERT_EQ(handle.GetValue(), 42U);
	}
}