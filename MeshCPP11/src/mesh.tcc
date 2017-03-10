/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:16:14-06:00
*/

/****************************************************

 Data structure c++11

 Mesh structure implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#include <stdexcept>

/****************************************************
 MNode::Private methods
*****************************************************/
/* Detach all */
template <class T, class KeyT>
int MNode<T, KeyT>::detach_all(enum MNodeDir dir)
{
	if (this->rh) this->rh->lh = nullptr;
	if (this->rd) this->rd->lu = nullptr;
	if (this->dn) this->dn->up = nullptr;
	if (this->lh) this->lh->rh = nullptr;
	if (this->lu) this->lu->rd = nullptr;
	if (this->up) this->up->dn = nullptr;

	switch (dir) {
	case RH:
		this->rh = nullptr;
		break;
	case RD:
		this->rd = nullptr;
		break;
	case DN:
		this->dn = nullptr;
		break;
	case LH:
		this->lh = nullptr;
		break;
	case LU:
		this->lu = nullptr;
		break;
	case UP:
		this->up = nullptr;
		break;
	}

	return (int)dir;
}

/* Attach */
template <class T, class KeyT>
int MNode<T, KeyT>::attach(
	enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn)
{
	assert(tn);

	switch (attach_direction) {
	case RH:
		this->rh = tn;
		update_adjs(RH, tn);
		break;
	case RD:
		this->rd = tn;
		update_adjs(RD, tn);
		break;
	case DN:
		this->dn = tn;
		update_adjs(DN, tn);
		break;
	case LH:
		this->lh = tn;
		update_adjs(LH, tn);
		break;
	case LU:
		this->lu = tn;
		update_adjs(LU, tn);
		break;
	case UP:
		this->up = tn;
		update_adjs(UP, tn);
		break;
	}

	return 0;
}

/* Scan left path */
template <class T, class KeyT>
int MNode<T, KeyT>::update_L(
	enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn)
{
	assert(tn);

	std::shared_ptr<MNode<T, KeyT>> tmp = tn;
	std::shared_ptr<MNode<T, KeyT>> tmp_n;
	int me;
	int tmp_dir = (int)attach_direction;

	while (true) {
		tmp_dir = find_L((enum MNodeDir)tmp_dir, tmp);
		if (tmp_dir == -1) break;

		tmp_n = map((enum MNodeDir)tmp_dir, tmp);

		if (tmp_n == tmp) break;
		else if (tmp_n == tn) break;
		else {
			me = find_me(tmp_n);
			if (me == -1) break;
			else {
				switch ((enum MNodeDir)me) {
				case RH: this->rh = tmp_n;
				case RD: this->rd = tmp_n;
				case DN: this->dn = tmp_n;
				case LH: this->lh = tmp_n;
				case LU: this->lu = tmp_n;
				case UP: this->up = tmp_n;
				}
			}
			tmp = tmp_n;
		}
	}

	return 0;
}

/* Scan right path */
template <class T, class KeyT>
int MNode<T, KeyT>::update_R(
	enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn)
{
	assert(tn);

	std::shared_ptr<MNode<T, KeyT>> tmp = tn;
	std::shared_ptr<MNode<T, KeyT>> tmp_n;
	int me;
	int tmp_dir = (int)attach_direction;

	while (true) {
		tmp_dir = find_R((enum MNodeDir)tmp_dir, tmp);
		if (tmp_dir == -1) break;

		tmp_n = map((enum MNodeDir)tmp_dir, tmp);

		if (tmp_n == tmp) break;
		else if (tmp_n == tn) break;
		else {
			me = find_me(tmp_n);
			if (me == -1) break;
			else {
				switch ((enum MNodeDir)me) {
				case RH: this->rh = tmp_n;
				case RD: this->rd = tmp_n;
				case DN: this->dn = tmp_n;
				case LH: this->lh = tmp_n;
				case LU: this->lu = tmp_n;
				case UP: this->up = tmp_n;
				}
			}
			tmp = tmp_n;
		}
	}

	return 0;
}

/* Scan adjs */
template <class T, class KeyT>
int MNode<T, KeyT>::update_adjs(
	enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn)
{
	return (update_L(attach_direction, tn)+update_R(attach_direction, tn));
}

/* Pathfinders */
template <class T, class KeyT>
int MNode<T, KeyT>::find_L(
	enum MNodeDir in_dir, std::shared_ptr<MNode<T, KeyT>> n)
{
	std::shared_ptr<MNode<T, KeyT>> retn;
	int i;

	switch (in_dir) {
	case RH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[0][i], n);
			if (retn) return i;
		}
		break;
	case RD:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[1][i], n);
			if (retn) return i;
		}
		break;
	case DN:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[2][i], n);
			if (retn) return i;
		}
		break;
	case LH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[3][i], n);
			if (retn) return i;
		}
		break;
	case LU:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[4][i], n);
			if (retn) return i;
		}
		break;
	case UP:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[5][i], n);
			if (retn) return i;
		}
		break;
	}

	return -1;
}

template <class T, class KeyT>
int MNode<T, KeyT>::find_R(
	enum MNodeDir in_dir, std::shared_ptr<MNode<T, KeyT>> n)
{
	std::shared_ptr<MNode<T, KeyT>> retn;
	int i;

	switch (in_dir) {
	case RH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[0][i], n);
			if (retn) return i;
		}
		break;
	case RD:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[1][i], n);
			if (retn) return i;
		}
		break;
	case DN:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[2][i], n);
			if (retn) return i;
		}
		break;
	case LH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[3][i], n);
			if (retn) return i;
		}
		break;
	case LU:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[4][i], n);
			if (retn) return i;
		}
		break;
	case UP:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[5][i], n);
			if (retn) return i;
		}
		break;
	}

	return -1;
}

template <class T, class KeyT>
int MNode<T, KeyT>::find_me(std::shared_ptr<MNode<T, KeyT>> m)
{
	if (m->rh) if (m->rh->GetKey() == this->GetKey()) return (int)RH;
	if (m->rd) if (m->rd->GetKey() == this->GetKey()) return (int)RD;
	if (m->dn) if (m->dn->GetKey() == this->GetKey()) return (int)DN;
	if (m->lh) if (m->lh->GetKey() == this->GetKey()) return (int)LH;
	if (m->lu) if (m->lu->GetKey() == this->GetKey()) return (int)LU;
	if (m->up) if (m->up->GetKey() == this->GetKey()) return (int)UP;

	return -1;
}

/* Map enum to node */
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::map(
	enum MNodeDir n, std::shared_ptr<MNode<T, KeyT>> nt)
{
	switch (n) {
	case RH:
		return nt->rh;
	case RD:
		return nt->rd;
	case DN:
		return nt->dn;
	case LH:
		return nt->lh;
	case LU:
		return nt->lu;
	case UP:
		return nt->up;
	}

	return nullptr;
}

/****************************************************
 MNode::Access node methods
*****************************************************/
template <class T, class KeyT>
int MNode<T, KeyT>::Set(T& d)
{
	std::shared_ptr<T> sd = std::make_shared<T>(d);
	assert(sd);
	this->data = sd;

	return 0;
}

template <class T, class KeyT>
int MNode<T, KeyT>::Set(std::shared_ptr<T> pd)
{
	this->data = pd;

	return 0;
}

template <class T, class KeyT>
T& MNode<T, KeyT>::Get()
{
	if (data == nullptr)
		throw std::invalid_argument(
			"MNode: nothing has been initialized!!");

	return *this->data;
}

template <class T, class KeyT>
std::shared_ptr<T> MNode<T, KeyT>::pGet()
{
	return this->data;
}

template <class T, class KeyT>
void MNode<T, KeyT>::SetKey(KeyT& k)
{
	this->key = k;
}

template <class T, class KeyT>
KeyT& MNode<T, KeyT>::GetKey()
{
	return this->key;
}

/****************************************************
 MNode::Manipulation
*****************************************************/
template <class T, class KeyT>
int MNode<T, KeyT>::SetRH(std::shared_ptr<MNode<T, KeyT>> mn)
{
	/* nullptr -> isolating this node from nodes */
	if (!mn) {
		this->detach_all(RH);
		return 0;
	}

	return this->attach(RH, mn);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetRD(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(RD);
		return 0;
	}

	return this->attach(RD, mn);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetDN(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(DN);
		return 0;
	}

	return this->attach(DN, mn);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLH(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(LH);
		return 0;
	}

	return this->attach(LH, mn);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLU(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(LU);
		return 0;
	}

	return this->attach(LU, mn);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetUP(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(UP);
		return 0;
	}

	return this->attach(UP, mn);
}

template <class T, class KeyT>
MNode<T, KeyT>& MNode<T, KeyT>::operator= (const MNode<T, KeyT>& mn)
{
	MNode<T, KeyT> tmp(mn);
	*this = std::move(tmp);
	return *this;
}

template <class T, class KeyT>
MNode<T, KeyT>& MNode<T, KeyT>::operator= (MNode<T, KeyT>&& mn) noexcept
{
	rh = nullptr;
	rd = nullptr;
	dn = nullptr;
	lh = nullptr;
	lu = nullptr;
	up = nullptr;
	data = nullptr;

	rh = mn.rh;
	rd = mn.rd;
	dn = mn.dn;
	lh = mn.lh;
	lu = mn.lu;
	up = mn.up;
	data = mn.data;

	mn.rh = nullptr;
	mn.rd = nullptr;
	mn.dn = nullptr;
	mn.lh = nullptr;
	mn.lu = nullptr;
	mn.up = nullptr;
	mn.data = nullptr;

	return *this;
}

/****************************************************
 MNode::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
MNode<T, KeyT>::MNode() :
	rh(nullptr),
	rd(nullptr),
	dn(nullptr),
	lh(nullptr),
	lu(nullptr),
	up(nullptr),
	data(nullptr)
{
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(T& d, KeyT& k) : MNode()
{
	this->Set(d);
	this->SetKey(k);
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(std::shared_ptr<T> pd, KeyT& k) : MNode()
{
	this->Set(pd);
	this->SetKey(k);
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(const MNode<T, KeyT>& mn) :
	key(mn.key)
{
	MNode<T, KeyT>& tmp_rh = *mn.rh;
	MNode<T, KeyT>& tmp_rd = *mn.rd;
	MNode<T, KeyT>& tmp_dn = *mn.dn;
	MNode<T, KeyT>& tmp_lh = *mn.lh;
	MNode<T, KeyT>& tmp_lu = *mn.lu;
	MNode<T, KeyT>& tmp_up = *mn.up;
	T& tmp = *mn.data;

	rh = std::make_shared<MNode<T, KeyT>>(tmp_rh);
	rd = std::make_shared<MNode<T, KeyT>>(tmp_rd);
	dn = std::make_shared<MNode<T, KeyT>>(tmp_dn);
	lh = std::make_shared<MNode<T, KeyT>>(tmp_lh);
	lu = std::make_shared<MNode<T, KeyT>>(tmp_lu);
	up = std::make_shared<MNode<T, KeyT>>(tmp_up);
	data = std::make_shared<T>(tmp);
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(MNode<T, KeyT>&& mn) noexcept : \
	rh(mn.rh),
	rd(mn.rd),
	dn(mn.dn),
	lh(mn.lh),
	lu(mn.lu),
	up(mn.up),
	key(mn.key)
{
	MNode<T, KeyT>& tmp_rh = *mn.rh;
	MNode<T, KeyT>& tmp_rd = *mn.rd;
	MNode<T, KeyT>& tmp_dn = *mn.dn;
	MNode<T, KeyT>& tmp_lh = *mn.lh;
	MNode<T, KeyT>& tmp_lu = *mn.lu;
	MNode<T, KeyT>& tmp_up = *mn.up;
	T& tmp = *mn.data;

	rh = std::make_shared<MNode<T, KeyT>>(tmp_rh);
	rd = std::make_shared<MNode<T, KeyT>>(tmp_rd);
	dn = std::make_shared<MNode<T, KeyT>>(tmp_dn);
	lh = std::make_shared<MNode<T, KeyT>>(tmp_lh);
	lu = std::make_shared<MNode<T, KeyT>>(tmp_lu);
	up = std::make_shared<MNode<T, KeyT>>(tmp_up);
	data = std::make_shared<T>(tmp);

	mn.rh = nullptr;
	mn.rd = nullptr;
	mn.dn = nullptr;
	mn.lh = nullptr;
	mn.lu = nullptr;
	mn.up = nullptr;
	mn.data = nullptr;
}

template <class T, class KeyT>
MNode<T, KeyT>::~MNode()
{
}
