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
		this->rh = tn;
		update_adjs(RD, tn);
		break;
	case DN:
		this->rh = tn;
		update_adjs(DN, tn);
		break;
	case LH:
		this->rh = tn;
		update_adjs(LH, tn);
		break;
	case LU:
		this->rh = tn;
		update_adjs(LU, tn);
		break;
	case UP:
		this->rh = tn;
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
		tmp_dir = find_L(tmp_dir, tmp);
		if (tmp_dir == -1) break;

		tmp_n = map(tmp_dir, tmp);

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
	enum MNodeDir tmp_dir = attach_direction;

	while (true) {
		tmp_dir = find_R(tmp_dir, tmp);
		if (tmp_dir == -1) break;

		tmp_n = map(tmp_dir, tmp);

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
			retn = map(MPathFL[0][i]);
			if (retn) return i;
		}
		break;
	case RD:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[1][i]);
			if (retn) return i;
		}
		break;
	case DN:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[2][i]);
			if (retn) return i;
		}
		break;
	case LH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[3][i]);
			if (retn) return i;
		}
		break;
	case LU:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[4][i]);
			if (retn) return i;
		}
		break;
	case UP:
		for (i=0; i<6; ++i) {
			retn = map(MPathFL[5][i]);
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
			retn = map(MPathFR[0][i]);
			if (retn) return i;
		}
		break;
	case RD:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[1][i]);
			if (retn) return i;
		}
		break;
	case DN:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[2][i]);
			if (retn) return i;
		}
		break;
	case LH:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[3][i]);
			if (retn) return i;
		}
		break;
	case LU:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[4][i]);
			if (retn) return i;
		}
		break;
	case UP:
		for (i=0; i<6; ++i) {
			retn = map(MPathFR[5][i]);
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
void MNode<T, KeyT>::Set(T& d)
{
	std::shared_ptr<T> sd = \
		std::make_shared<T>(d);
	assert(sd);
	this->data = sd;
}

template <class T, class KeyT>
T MNode<T, KeyT>::Get() const
{
	assert(this->data);
	return *this->data;
}

template <class T, class KeyT>
std::shared_ptr<T> MNode<T, KeyT>::pGet() const
{
	return this->data;
}

template <class T, class KeyT>
void MNode<T, KeyT>::SetKey(KeyT& k)
{
	this->key = k;
}

template <class T, class KeyT>
KeyT MNode<T, KeyT>::GetKey() const
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

	/* updating rd */
	return this->attach(RH);
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetRD(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(RD);
		return 0;
	}


}

template <class T, class KeyT>
int MNode<T, KeyT>::SetDN(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(DN);
		return 0;
	}

}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLH(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(LH);
		return 0;
	}

}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLU(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(LU);
		return 0;
	}

}

template <class T, class KeyT>
int MNode<T, KeyT>::SetUP(std::shared_ptr<MNode<T, KeyT>> mn)
{
	if (!mn) {
		this->detach_all(UP);
		return 0;
	}

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
MNode<T, KeyT>::MNode(MNode<T, KeyT>& mn) :
	rh(mn.rh),
	rd(mn.rd),
	dn(mn.dn),
	lh(mn.lh),
	lu(mn.lu),
	up(mn.up),
	data(mn.data),
	key(mn.key)
{
}

template <class T, class KeyT>
MNode<T, KeyT>::~MNode()
{
}
