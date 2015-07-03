//Un arbol Ordenado Orientado
//Tiene una modificacion de la estructura de DashStructures para simplificar el recorrido de
//un nodo a sus padres, lo que hace es exponer el nodo padre el nodo y el iterador
#pragma once

#ifndef DASH_TREE

#define DASH_TREE

#include "CommonTypes.h"

#include "Utility.h"


#ifndef NOT_NAMESPACE_DASH_STRUCTURES_IN_ALL_CPPS
_BEGIN_DASH_STRUCTURES_NAMESPACE
#endif


template<class _Tp>
class DTree;
template<class _Tp>
class _Tree_Iterator;
template<class _Tp>
class _DTree_Node;

class _DTree_Node_base{

protected:
	_DTree_Node_base* father;
	_DTree_Node_base* right;
	_DTree_Node_base* left_child;

	_DTree_Node_base(_DTree_Node_base* _right, _DTree_Node_base* _left_child, _DTree_Node_base* _father): 
		right(_right), left_child(_left_child), father(_father){}

	};

	template<class _Tp>
class _DTree_Node:public _DTree_Node_base{

	friend class DTree<_Tp>;
	friend class _Tree_Iterator<_Tp>;

	_Tp data;

	_DTree_Node(): data( _Tp() ) ,_DTree_Node_base(NULL,NULL,NULL){}
	

	};


	template<class _Tp>
class _Tree_Iterator{

	friend class DTree<_Tp>;

private:

	typedef _DTree_Node<_Tp> _Node;

	_Node* ptr;
	_Node* prev;
	_Node* father;

	_Tree_Iterator(_Node* _ptr, _Node* _prev ,_Node* _father): ptr(_ptr), prev(_prev), father(_father){}

public:

	_Tree_Iterator( const _Tree_Iterator& cpy ): ptr(cpy.ptr), prev(cpy.prev), father(cpy.father){}

	_Tree_Iterator(): ptr(NULL), prev(NULL), father(NULL){}


	_Tp& operator*(){
		return ptr->data;
		}

	_Tp* operator->(){
		return &ptr->data;
		}

	bool operator==( const _Tree_Iterator& _B ) const{
		return ( this->ptr == _B.ptr );
		}

	bool operator!=( const _Tree_Iterator& _B ) const{
		return ( this->ptr != _B.ptr );
		}

	//El nombre raro es porque es el nodo padre lo que devuelve pero no es un nodo con informacion completa
	//no se puede ver cual era el anterior de ese nodo y devido a eso no se puede hacer INSERSIONES cuando se
	//recorren los padres!!!, pero se se puede transversar el camino de los padres :D
	_Tree_Iterator onlyFather(){
		return _Tree_Iterator( static_cast<_Node*>(this->ptr->father) , NULL , NULL );	//Aca esta la limitacion de la modificacion, no puero
																						//llevar el anterior de los padres, pero eso no lo necesito
		}

	_Tree_Iterator right(){
		return _Tree_Iterator( static_cast<_Node*>(this->ptr->right) , this->ptr , this->father );
		}

	_Tree_Iterator lchild(){
		return _Tree_Iterator( static_cast<_Node*>(this->ptr->left_child) , NULL, this->ptr );
		}

	_Tree_Iterator operator++(){
		*this = this->right();
		return *this;
		}

	_Tree_Iterator operator++(int){
		_Tree_Iterator _aux = *this;
		*this = this->right();
		return _aux;
		}

	};



	template<class _Tp>
class DTree{
public:

typedef _Tp value_type;
typedef _Tree_Iterator<_Tp> iterator;
typedef const _Tree_Iterator<_Tp> const_iterator;
typedef _Tp* pointer;
typedef const _Tp* const_pointer;
typedef _Tp& reference;
typedef const _Tp& const_reference;
typedef _DTree_Node<_Tp> _Node;
typedef size_t size_type;
typedef ptrdiff_t difference_type;

private:

	_Node Header;

	iterator tree_copy_aux( iterator a, DTree& _B, iterator b){
		a = this->insert(a, *b);

		iterator ac= a.lchild(),
			bc= b.lchild();
		while(bc != _B.end()){
			ac = tree_copy_aux(ac,_B,bc);
			ac++;
			bc++;
			}

		return a;

		}

public:

	DTree(){
		Header.data = value_type();
		Header.right = NULL ;
		Header.left_child = NULL ;
		}

	DTree(DTree<_Tp>& _B){
		if( this != (&_B)){
			Header.data = value_type();
			Header.right = NULL ;
			Header.left_child = NULL ;
			
			if( _B.begin() != _B.end() )
				tree_copy_aux( this->begin() , _B, _B.begin() );
			}
		}

	~DTree(){
		this->clear();
		}

	DTree& operator=(DTree<_Tp>& _B){
		if( this != (&_B) ){
			this->clear();
			tree_copy_aux( this->begin() , _B, _B.begin() );

			}
		return (*this);
	
		}

	iterator begin(){
		return iterator( static_cast<_Node*>(Header.left_child) , NULL , &Header );
		}

	iterator end(){
		return iterator();
		}

	iterator insert( iterator _p , value_type _D){

		//lo nuevo
		_Node* novo = new _Node();

		novo->data= _D;

		novo->father = _p.father;//ESTA ES UNA LINEA MODIFICADA

		novo->right = _p.ptr;
		novo->left_child = NULL;

		_p.ptr = novo;

		if( _p.prev ){
			_p.prev->right = novo;
			}else{
				_p.father->left_child = novo;
			}

		return _p;
		}

	iterator erase( iterator _p ){
		if( _p == this->end() )
			return _p;

		
		iterator q = _p.lchild();
		while( q != this->end() ){
			q = erase(q);
			}

		_Node* whomlldie = _p.ptr;
		_p.ptr = (_Node*)_p.ptr->right;

		if( _p.prev ){
			_p.prev->right = _p.ptr;
			}else{
				_p.father->left_child = _p.ptr;
			}
		//la accion que se encarga de borrar
		delete whomlldie;

		return _p;
		}

	void clear(){
		this->erase( this->begin() );
		}

	iterator splice( iterator to, iterator from){

		if( from.prev ){
		from.prev->right = from.ptr->right;
			}else{
			from.father->left_child = from.ptr->right;
			}

		from.ptr->right = to.ptr;
		to.ptr = from.ptr;//para devolverlo despues

		if( to.prev ){
		to.prev->right = from.ptr;
			}else{
			to.father->left_child = from.ptr;
			}

		return to;
		}

	iterator find( value_type _val, iterator _p ){
		if( _p == this->end() || (*_p) == _val) 
			return _p;
		
		
		iterator aux, q = _p.lchild();
		while( q != this->end() ){
			aux = find( _val, q );
			if( aux != this->end() ) return aux;

			q++;
			}

		return iterator();

		}

		iterator find( value_type _val ){
			return find( _val, this->begin() );
			}


		};


#ifndef NOT_NAMESPACE_DASH_STRUCTURES_IN_ALL_CPPS
_END_DASH_STRUCTURES_NAMESPACE
#endif



#endif