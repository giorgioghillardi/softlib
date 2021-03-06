#ifndef _CONNOR_HASTIE_DISTRIBUTION_H
#define _CONNOR_HASTIE_DISTRIBUTION_H

#include <softlib/DistributionFunction/MomentumSpaceDistributionFunction.h>

class ConnorHastieDistribution : public MomentumSpaceDistributionFunction {
    protected:
        slibreal_t EHat, Zeff, pMax, deltaP;
        slibreal_t D;
    public:
        ConnorHastieDistribution();
        ConnorHastieDistribution(const slibreal_t, const slibreal_t, const slibreal_t pMax=0, const slibreal_t deltaP=0);

        using MomentumSpaceDistributionFunction::Eval;

        virtual slibreal_t Eval(const slibreal_t, const slibreal_t) override;
        void Initialize(const slibreal_t, const slibreal_t, const slibreal_t pMax=0, const slibreal_t deltaP=0);
        virtual ConnorHastieDistribution *MinClone() override;
};

#endif/*_CONNOR_HASTIE_DISTRIBUTION_H*/
