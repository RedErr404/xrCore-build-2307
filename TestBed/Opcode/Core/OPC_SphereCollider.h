///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for a sphere collider.
 *	\file		OPC_SphereCollider.h
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_SPHERECOLLIDER_H__
#define __OPC_SPHERECOLLIDER_H__

	class OPCODE_API AABBSphereCollider : public Collider
	{
		public:
		// Constructor / Destructor
												AABBSphereCollider();
		virtual									~AABBSphereCollider();
		// Generic collision query

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Generic collision query for generic OPCODE models. After the call, access the results:
		 *	- with GetContactStatus()
		 *	- in the user-provided destination array
		 *
		 *	\param		sphere			[in] collision sphere in local space
		 *	\param		model			[in] Opcode model to collide with
		 *	\param		worlds			[in] sphere's world matrix
		 *	\param		worldm			[in] model's world matrix
		 *	\param		cache			[in] a possibly cached face index, or null
		 *	\return		true if success
		 *	\warning	SCALE NOT SUPPORTED. The matrices must contain rotation & translation parts only.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool					Collide(const Sphere& sphere, OPCODE_Model* model, const Matrix4x4& worlds, const Matrix4x4& worldm, udword* cache=null);

		// Collision queries
						bool					Collide(const Sphere& sphere, const AABBCollisionTree* tree, const Matrix4x4& worlds, const Matrix4x4& worldm, udword* cache=null);
						bool					Collide(const Sphere& sphere, const AABBNoLeafTree* tree, const Matrix4x4& worlds, const Matrix4x4& worldm, udword* cache=null);
						bool					Collide(const Sphere& sphere, const AABBQuantizedTree* tree, const Matrix4x4& worlds, const Matrix4x4& worldm, udword* cache=null);
						bool					Collide(const Sphere& sphere, const AABBQuantizedNoLeafTree* tree, const Matrix4x4& worlds, const Matrix4x4& worldm, udword* cache=null);
		// Settings

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Settings: sets the destination array for stabbed faces.
		 *	\param		cf			[in] destination array, filled during queries
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	void					SetDestination(CollisionFaces* cf)		{ mFaces	= cf;								}

		// Stats

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Stats: gets the number of Sphere-BV overlap tests after a collision query.
		 *	\see		GetNbSpherePrimTests()
		 *	\return		the number of Sphere-BV tests performed during last query
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	udword					GetNbSphereBVTests()			const	{ return mNbSphereBVTests;						}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Stats: gets the number of Sphere-Triangle overlap tests after a collision query.
		 *	\see		GetNbSphereBVTests()
		 *	\return		the number of Sphere-Triangle tests performed during last query
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	udword					GetNbSpherePrimTests()			const	{ return mNbSpherePrimTests;					}

		// Callback control

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Callback control: a method to setup user-data assigned to object callback.
		 *	\param		data		[in] user-defined data
		 *	\return		Self-Reference
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	AABBSphereCollider&		SetUserData(udword data)				{ mUserData	= data;				return *this;	}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Callback control: a method to setup object callback. Must provide triangle-vertices for a given triangle index.
		 *	\param		callback	[in] user-defined callback
		 *	\return		Self-Reference
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	AABBSphereCollider&		SetCallbackObj(OPC_CALLBACK callback)	{ mObjCallback	= callback;		return *this;	}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Validates current settings. You should call this method after all the settings and callbacks have been defined for a collider.
		 *	\return		null if everything is ok, else a string describing the problem
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		override(Collider)	const char*			ValidateSettings();

		private:
		// Sphere in local space
							Point				mCenter;			//!< Sphere center
							float				mRadius2;			//!< Sphere radius squared
		// Colliding faces
							CollisionFaces*		mFaces;				//!< List of collided faces
		// User callback
							udword				mUserData;			//!< User-defined data sent to callback
							OPC_CALLBACK		mObjCallback;		//!< Object callback
		// Stats
							udword				mNbSphereBVTests;	//!< Number of Sphere-BV tests
							udword				mNbSpherePrimTests;	//!< Number of Sphere-Primitive tests
		// Dequantization coeffs
							Point				mCenterCoeff;
							Point				mExtentsCoeff;
		// Internal methods
							void				_Collide(const AABBCollisionNode* node);
							void				_Collide(const AABBNoLeafNode* node);
							void				_Collide(const AABBQuantizedNode* node);
							void				_Collide(const AABBQuantizedNoLeafNode* node);
			// Overlap tests
							bool				SphereAABBOverlap(const Point& center, const Point& extents);
							bool				SphereTriOverlap(const Point& vert0, const Point& vert1, const Point& vert2, float& sqrdist, float& u, float& v);
			// Init methods
							void				InitQuery(const Sphere& sphere, const Matrix4x4& worlds, const Matrix4x4& worldm);
							bool				CheckTemporalCoherence(udword* faceid);
	};

#endif // __OPC_SPHERECOLLIDER_H__
