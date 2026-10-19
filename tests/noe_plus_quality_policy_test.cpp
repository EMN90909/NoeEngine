// Standalone policy tests (native C++17 compiler, no GPU needed).
#include "main/noe_render_quality_policy.h"
#include <cassert>
#include <cmath>
using namespace NoePlus;
int main() {
    assert(resolve(Tier::AUTO,true,16ULL<<30)==Tier::LITE);
    assert(resolve(Tier::AUTO,false,6ULL<<30)==Tier::LITE);
    assert(resolve(Tier::AUTO,false,16ULL<<30)==Tier::BALANCED);
    assert(resolve(Tier::ULTRA,true,16ULL<<30)==Tier::LITE);
    assert(resolve(Tier::CUSTOM,true,16ULL<<30)==Tier::CUSTOM);
    assert(preset(Tier::LITE,true).scale==1.0f);
    assert(preset(Tier::LITE,true).scaling_mode==0);
    assert(preset(Tier::LITE,false).scaling_mode==1);
    assert(preset(Tier::HIGH,false).shadow_atlas==4096);
    assert(preset(Tier::ULTRA,false).msaa_3d==3);
    AdaptiveScale s;
    s.reset(.85f,.65f,1.0f,16.667);
    float out=0;
    assert(!s.sample(-1,&out) && !s.sample(NAN,&out) && !s.sample(16,nullptr));
    bool slowed=false;
    for(int i=0;i<1500;i++) slowed|=s.sample(33.3,&out);
    assert(slowed && s.current_scale()>=.65f && s.current_scale()<.85f);
    bool recovered=false;
    for(int i=0;i<2500;i++) recovered|=s.sample(5.0,&out);
    assert(recovered && s.current_scale()<=1.f && s.current_scale()>.65f);
}
