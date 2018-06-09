//
// Created by Vesper on 6/9/18.
//

namespace feature {
    template<typename T>
    struct Feature {
        static const bool value = true;
    };

    template<typename... Features>
    struct FeatureList {
    };

    template<typename T, typename... Features>
    constexpr bool ContainsFeature = false;

    template<typename T, typename... Rest>
    constexpr bool ContainsFeature<T, T, Rest...> = true;

    template<typename T, typename J, typename... Rest>
    constexpr bool ContainsFeature<T, J, Rest...> = ContainsFeature<T, Rest...>;

    template<typename T, typename FeatureList>
    constexpr bool ListContainsFeature = false;

    template<typename T, typename... Features>
    constexpr bool ListContainsFeature<T, FeatureList<Features...>> = ContainsFeature<T, Features...>;
};

int main() {
using namespace feature;
#define DEFINE_FEATURE(NAME)\
typedef struct _##NAME {} NAME;

#define DEFINE_CONFIG(NAME,A,...)\
typedef FeatureList<A,##__VA_ARGS__> FeatureList_##NAME;

    DEFINE_FEATURE(A);
    DEFINE_FEATURE(B);
    DEFINE_FEATURE(C);
    DEFINE_FEATURE(D);
    DEFINE_FEATURE(E);
    DEFINE_CONFIG(NewProdcut, A, B,C,D,E);
    DEFINE_CONFIG(ExpiredProduct,A,B);
    DEFINE_CONFIG(CurrentProduct, A,B,D);
    DEFINE_CONFIG(OldProduct,A,B,C);


    FeatureList_NewProdcut ne;
    FeatureList_ExpiredProduct exp;
    FeatureList_CurrentProduct curr;
    FeatureList_OldProduct  old;
    static_assert(ListContainsFeature<A,FeatureList_NewProdcut>);
    static_assert(ListContainsFeature<B,FeatureList_NewProdcut>);
    static_assert(!ListContainsFeature<C,FeatureList_ExpiredProduct>);
    static_assert(!ListContainsFeature<D,FeatureList_ExpiredProduct>);
    static_assert(!ListContainsFeature<E,FeatureList_OldProduct>);
    static_assert(ListContainsFeature<B,FeatureList_OldProduct>);
    static_assert(ListContainsFeature<D,FeatureList_CurrentProduct>);

}