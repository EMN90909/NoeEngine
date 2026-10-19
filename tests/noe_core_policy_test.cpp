#include "main/noe_core_policy.h"
#include <cassert>
#include <limits>
int main() {
    using namespace NoeCore;
    Hardware h;
    assert(recommend(h) == Profile::LOW);
    h = { 8, 16 * GiB, 4 * GiB, true, true };
    assert(recommend(h) == Profile::HIGH);
    h = { 16, 32 * GiB, 12 * GiB, true, true };
    assert(recommend(h) == Profile::ULTRA);
    h = { 8, 16 * GiB, 0, true, true };
    assert(recommend(h) == Profile::BALANCED);
    h = { 8, 16 * GiB, 8 * GiB, true, false };
    assert(recommend(h) == Profile::LOW);
    h = { 2, 32 * GiB, 8 * GiB, true, true };
    assert(recommend(h) == Profile::LOW);
    MemoryBudget budget(100);
    CacheEntry item{ 60, 10, 0, false, false, false };
    budget.account(101);
    assert(budget.over_budget());
    assert(!budget.may_evict(item, 129));
    assert(budget.may_evict(item, 130));
    item.in_active_scene = true;
    assert(!budget.may_evict(item, 10000));
    item.in_active_scene = false;
    item.external_references = 1;
    assert(!budget.may_evict(item, 10000));
    item.external_references = 0;
    item.pinned = true;
    assert(!budget.may_evict(item, 10000));
    item.pinned = false;
    item.loading = true;
    assert(!budget.may_evict(item, 10000));
    item.loading = false;
    budget.release(101);
    assert(!budget.may_evict(item, 10000));
    budget.account(std::numeric_limits<uint64_t>::max());
    budget.account(42);
    assert(budget.usage() == std::numeric_limits<uint64_t>::max());
    budget.release(std::numeric_limits<uint64_t>::max());
    assert(budget.usage() == 0);
    return 0;
}
