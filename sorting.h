#ifndef SORTING_H
#define SORTING_H
//#include "PrimerPair.h"





 /** Compare two ClassType objects by their data member.*/
 template < typename ClassType, typename MemberValueType, typename FuncApplyOnMember >
 struct CmpByMemberFunctorAscend {
     /*Pointer to the member to be compared.*/
     MemberValueType ClassType::* _pointerToMember ;
     /* A functor, which will be applid on the data member before the comparing.*/
     FuncApplyOnMember _f ;

     /* Constructor
      pm: Pointer to data member
      f: Functor applyed on data member before the comparing.*/
     CmpByMemberFunctorAscend(MemberValueType ClassType::* pm, FuncApplyOnMember f) : _pointerToMember(pm), _f(f) {
     }

     /*Compare two objects, call by std::sort()*/
     bool operator()(ClassType* p1, ClassType* p2) const {
         // Here, _f will be applied on the data member.
         // _f might be a __Self_Ref, which just does nothing,
         //          or a byIndex, which will treat p1.*_pointerToMember as an array/vector, and return nth element.
         return _f(p1->*_pointerToMember) < _f(p2->*_pointerToMember) ;
     }
 } ;
 template < typename ClassType, typename MemberValueType, typename FuncApplyOnMember >
 struct CmpByMemberFunctorDescend {
     MemberValueType ClassType::* _pointerToMember ;
     FuncApplyOnMember _f ;

     CmpByMemberFunctorDescend(MemberValueType ClassType::* pm, FuncApplyOnMember f) : _pointerToMember(pm), _f(f) {
     }

     bool operator()(ClassType* p1, ClassType* p2) const {
         return _f(p1->*_pointerToMember) > _f(p2->*_pointerToMember) ;
     }
 } ;

 /*A dummy functor, actually does nothing.*/
 struct __Self_Ref {
     template < typename T >
     T& operator()(T& v) const { return v; }
 } ;

 /* A functor, will retrieve the nth element.*/
 struct byIndex {
     int _idx ;
     byIndex(int idx) : _idx(idx) {}
     template < typename T >
     const typename T::value_type& operator()(const T& v) const { return v[_idx] ; }
 } ;

 /*A helper function to create a CmpByMemberFunctor object.
v The pointer-to-member to be compared.*/
 template < typename ClassType, typename MemberValueType >
 CmpByMemberFunctorAscend<ClassType, MemberValueType, __Self_Ref> byMemberAscend(MemberValueType ClassType::* v) {
     __Self_Ref self_ref ;
     return CmpByMemberFunctorAscend<ClassType, MemberValueType, __Self_Ref>(v, self_ref) ;
 }
 template < typename ClassType, typename MemberValueType >
 CmpByMemberFunctorDescend<ClassType, MemberValueType, __Self_Ref> byMemberDescend(MemberValueType ClassType::* v) {
     __Self_Ref self_ref ;
     return CmpByMemberFunctorDescend<ClassType, MemberValueType, __Self_Ref>(v, self_ref) ;
 }
 
 

 /* A helper function to create a CmpByMemberFunctor object.
v: The pointer-to-member to be compared.
f: Functor applyed on data member before the comparing.*/
 template < typename ClassType, typename MemberValueType, typename F >
 CmpByMemberFunctorAscend<ClassType, MemberValueType, F> byMemberAscend(MemberValueType ClassType::* v, F f) {
     return CmpByMemberFunctorAscend<ClassType, MemberValueType, F>(v, f) ;
 }
 template < typename ClassType, typename MemberValueType, typename F >
 CmpByMemberFunctorDescend<ClassType, MemberValueType, F> byMemberDescend(MemberValueType ClassType::* v, F f) {
     return CmpByMemberFunctorDescend<ClassType, MemberValueType, F>(v, f) ;
 }
#endif // SORTING_H
