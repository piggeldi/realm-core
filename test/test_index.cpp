#include "testsettings.hpp"
#ifdef TEST_INDEX

#include <tightdb/index.hpp>

#include "util/unit_test.hpp"
#include "util/test_only.hpp"

using namespace tightdb;

TEST(Index_Test1)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(0);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    CHECK_EQUAL(0, ndx.find_first(3));
    CHECK_EQUAL(1, ndx.find_first(100));
    CHECK_EQUAL(2, ndx.find_first(10));
    CHECK_EQUAL(3, ndx.find_first(45));
    CHECK_EQUAL(4, ndx.find_first(0));

    // Clean up
    col.Destroy();
    ndx.Destroy();
}

TEST(Index_FindAll)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(0);
    col.add(10);
    col.add(18);
    col.add(10);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    Column result;
    ndx.find_all(result, 10);

    CHECK_EQUAL(3, result.Size());

    // we need the refs sorted to verify
    result.sort();

    CHECK_EQUAL(2, result.Get(0));
    CHECK_EQUAL(5, result.Get(1));
    CHECK_EQUAL(7, result.Get(2));

    // Clean up
    result.Destroy();
    col.Destroy();
    ndx.Destroy();
}

TEST(Index_FindAllRange)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(0);
    col.add(10);
    col.add(18);
    col.add(10);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    Column result;
    ndx.FindAllRange(result, 10, 50);

    CHECK_EQUAL(5, result.Size());

    // we need the refs sorted to verify
    result.sort();

    CHECK_EQUAL(2, result.Get(0)); // 10
    CHECK_EQUAL(3, result.Get(1)); // 45
    CHECK_EQUAL(5, result.Get(2)); // 10
    CHECK_EQUAL(6, result.Get(3)); // 10
    CHECK_EQUAL(7, result.Get(4)); // 18

    // Clean up
    result.Destroy();
    col.Destroy();
    ndx.Destroy();
}

TEST(Index_Delete)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(0);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    // Delete first item (in index)
    ndx.Delete(4, 0, true); // opt for last item

    CHECK_EQUAL(0, ndx.find_first(3));
    CHECK_EQUAL(1, ndx.find_first(100));
    CHECK_EQUAL(2, ndx.find_first(10));
    CHECK_EQUAL(3, ndx.find_first(45));
    CHECK_EQUAL(-1, ndx.find_first(0));

    // Delete last item (in index)
    ndx.Delete(1, 100);

    CHECK_EQUAL(0, ndx.find_first(3));
    CHECK_EQUAL(1, ndx.find_first(10));
    CHECK_EQUAL(2, ndx.find_first(45));
    CHECK_EQUAL(-1, ndx.find_first(100));

    // Delete middle item (in index)
    ndx.Delete(1, 10);

    CHECK_EQUAL(0, ndx.find_first(3));
    CHECK_EQUAL(1, ndx.find_first(45));
    CHECK_EQUAL(-1, ndx.find_first(10));

    // Delete all items
    ndx.Delete(1, 45);
    ndx.Delete(0, 3);

    CHECK_EQUAL(-1, ndx.find_first(3));
    CHECK_EQUAL(-1, ndx.find_first(45));
    CHECK_EQUAL(true, ndx.is_empty());

    // Clean up
    col.Destroy();
    ndx.Destroy();
}

TEST(Index_Insert)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(1);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    // Insert item in top of column
    ndx.Insert(0, 0);

    CHECK_EQUAL(0, ndx.find_first(0));
    CHECK_EQUAL(1, ndx.find_first(3));
    CHECK_EQUAL(2, ndx.find_first(100));
    CHECK_EQUAL(3, ndx.find_first(10));
    CHECK_EQUAL(4, ndx.find_first(45));
    CHECK_EQUAL(5, ndx.find_first(1));

    // Append item in end of column
    ndx.Insert(6, 300, true); // opt for last item

    CHECK_EQUAL(0, ndx.find_first(0));
    CHECK_EQUAL(1, ndx.find_first(3));
    CHECK_EQUAL(2, ndx.find_first(100));
    CHECK_EQUAL(3, ndx.find_first(10));
    CHECK_EQUAL(4, ndx.find_first(45));
    CHECK_EQUAL(5, ndx.find_first(1));
    CHECK_EQUAL(6, ndx.find_first(300));

    // Insert item in middle
    ndx.Insert(3, 15);

    CHECK_EQUAL(0, ndx.find_first(0));
    CHECK_EQUAL(1, ndx.find_first(3));
    CHECK_EQUAL(2, ndx.find_first(100));
    CHECK_EQUAL(3, ndx.find_first(15));
    CHECK_EQUAL(4, ndx.find_first(10));
    CHECK_EQUAL(5, ndx.find_first(45));
    CHECK_EQUAL(6, ndx.find_first(1));
    CHECK_EQUAL(7, ndx.find_first(300));

    // Clean up
    col.Destroy();
    ndx.Destroy();
}

TEST(Index_Set)
{
    // Create a column with random values
    Column col;
    col.add(3);
    col.add(100);
    col.add(10);
    col.add(45);
    col.add(0);

    // Create a new index on column
    Index ndx;
    ndx.BuildIndex(col);

    // Set top value
    ndx.Set(0, 3, 4);

    CHECK_EQUAL(-1, ndx.find_first(3));
    CHECK_EQUAL(0, ndx.find_first(4));
    CHECK_EQUAL(1, ndx.find_first(100));
    CHECK_EQUAL(2, ndx.find_first(10));
    CHECK_EQUAL(3, ndx.find_first(45));
    CHECK_EQUAL(4, ndx.find_first(0));

    // Set bottom value
    ndx.Set(4, 0, 300);

    CHECK_EQUAL(-1, ndx.find_first(0));
    CHECK_EQUAL(0, ndx.find_first(4));
    CHECK_EQUAL(1, ndx.find_first(100));
    CHECK_EQUAL(2, ndx.find_first(10));
    CHECK_EQUAL(3, ndx.find_first(45));
    CHECK_EQUAL(4, ndx.find_first(300));

    // Set middle value
    ndx.Set(2, 10, 200);

    CHECK_EQUAL(-1, ndx.find_first(10));
    CHECK_EQUAL(0, ndx.find_first(4));
    CHECK_EQUAL(1, ndx.find_first(100));
    CHECK_EQUAL(2, ndx.find_first(200));
    CHECK_EQUAL(3, ndx.find_first(45));
    CHECK_EQUAL(4, ndx.find_first(300));

    // Clean up
    col.Destroy();
    ndx.Destroy();
}
#endif
