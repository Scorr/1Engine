#include "CMotion.h"

template<>
CGameBehavior* CMotion::GetOwner ( void )
{
	if (ownerType == core::kBasetypeGameBehavior) {
		return static_cast<CGameBehavior*>(owner);
	}
	return NULL;
}
template<>
IPrGameMotion* CMotion::GetOwner ( void )
{
	if (ownerType == core::kBasetypeIPrMotion) {
		return static_cast<IPrGameMotion*>(owner);
	}
	return NULL;
}
template<>
RrLogicObject* CMotion::GetOwner ( void )
{
	if (ownerType == core::kBasetypeRrLogicObject) {
		return static_cast<RrLogicObject*>(owner);
	}
	return NULL;
}
template<>
RrRenderObject* CMotion::GetOwner ( void )
{
	if (ownerType == core::kBasetypeRrRenderObject) {
		return static_cast<RrRenderObject*>(owner);
	}
	return NULL;
}


template <>
void CMotion::SetOwner ( CGameBehavior* n_owner )
{
	owner = static_cast<void*>(n_owner);
	ownerType = core::kBasetypeGameBehavior;
}
template <>
void CMotion::SetOwner ( IPrGameMotion* n_owner )
{
	owner = static_cast<void*>(n_owner);
	ownerType = core::kBasetypeIPrMotion;
}
template <>
void CMotion::SetOwner ( RrLogicObject* n_owner )
{
	owner = static_cast<void*>(n_owner);
	ownerType = core::kBasetypeRrLogicObject;
}
template <>
void CMotion::SetOwner ( RrRenderObject* n_owner )
{
	owner = static_cast<void*>(n_owner);
	ownerType = core::kBasetypeRrRenderObject;
}