/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Generated from Revision: 1.73 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Coord3fQuat
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Coord3fQuat(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__();

   public Coord3fQuat()
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__gmtl_Coord3fQuat([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0);

   public Coord3fQuat(gmtl.Coord3fQuat p0)
   {
      
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__gmtl_Coord3fQuat(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__gmtl_Vec3f_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public Coord3fQuat(gmtl.Vec3f p0, gmtl.Quatf p1)
   {
      
      
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__gmtl_Vec3f_gmtl_Quatf(p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5);

   public Coord3fQuat(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float(p0, p1, p2, p3, p4, p5);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6);

   public Coord3fQuat(float p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float_float(p0, p1, p2, p3, p4, p5, p6);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6,
	float p7);

   public Coord3fQuat(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____Coord__float_float_float_float_float_float_float_float(p0, p1, p2, p3, p4, p5, p6, p7);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Coord3fQuat(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Coord3fQuat(IntPtr obj);

   // Destructor.
   ~Coord3fQuat()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Coord3fQuat(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____getPos__0(IntPtr obj);

   public  gmtl.Vec3f getPos()
   {
      gmtl.Vec3f result;
      result = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____getPos__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____getRot__0(IntPtr obj);

   public  gmtl.Quatf getRot()
   {
      gmtl.Quatf result;
      result = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____getRot__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____pos__0(IntPtr obj);

   public  gmtl.Vec3f pos()
   {
      gmtl.Vec3f result;
      result = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____pos__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____rot__0(IntPtr obj);

   public  gmtl.Quatf rot()
   {
      gmtl.Quatf result;
      result = gmtl_Coord_gmtl_Vec_float_3__gmtl_Quat_float____rot__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Coord<gmtl.Vec<float,3>,gmtl.Quat<float> >.Params.
   public enum Params
   {
      RotSize = 4, PosSize = 3
   };


} // class gmtl.Coord3fQuat

/// <summary>
/// Custom marshaler for gmtl.Coord3fQuat.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Coord3fQuatMarshaler : ICustomMarshaler
{
   public void CleanUpManagedData(Object obj)
   {
   }

   public void CleanUpNativeData(IntPtr nativeData)
   {
   }

   public int GetNativeDataSize()
   {
      return -1;
   }

   // Marshaling for managed data being passed to C++.
   public IntPtr MarshalManagedToNative(Object obj)
   {
      return ((gmtl.Coord3fQuat) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Coord3fQuat(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Coord3fQuatMarshaler mInstance = new Coord3fQuatMarshaler();
}


} // namespace gmtl
