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

// Generated from Revision: 1.70 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class AABoxf
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected AABoxf(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_float__AABox__();

   public AABoxf()
   {
      mRawObject   = gmtl_AABox_float__AABox__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_float__AABox__gmtl_Point3f_gmtl_Point3f(ref int p0,
	ref int p1);

   public AABoxf(ref int p0, ref int p1)
   {
      
      
      mRawObject   = gmtl_AABox_float__AABox__gmtl_Point3f_gmtl_Point3f(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_float__AABox__gmtl_AABoxf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0);

   public AABoxf(gmtl.AABoxf p0)
   {
      
      mRawObject   = gmtl_AABox_float__AABox__gmtl_AABoxf(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal AABoxf(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_AABoxf(IntPtr obj);

   // Destructor.
   ~AABoxf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_AABoxf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_AABox_float__getMin__(IntPtr obj);

   public  gmtl.Point3f getMin()
   {
      gmtl.Point3f result;
      result = gmtl_AABox_float__getMin__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_AABox_float__getMax__(IntPtr obj);

   public  gmtl.Point3f getMax()
   {
      gmtl.Point3f result;
      result = gmtl_AABox_float__getMax__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_AABox_float__isEmpty__(IntPtr obj);

   public  bool isEmpty()
   {
      bool result;
      result = gmtl_AABox_float__isEmpty__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_float__setMin__gmtl_Point3f(IntPtr obj,
	ref int p0);

   public  void setMin(ref int p0)
   {
      
      gmtl_AABox_float__setMin__gmtl_Point3f(mRawObject, ref p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_float__setMax__gmtl_Point3f(IntPtr obj,
	ref int p0);

   public  void setMax(ref int p0)
   {
      
      gmtl_AABox_float__setMax__gmtl_Point3f(mRawObject, ref p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_float__setEmpty__bool(IntPtr obj,
	bool p0);

   public  void setEmpty(bool p0)
   {
      gmtl_AABox_float__setEmpty__bool(mRawObject, p0);
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.AABoxf

/// <summary>
/// Custom marshaler for gmtl.AABoxf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AABoxfMarshaler : ICustomMarshaler
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
      return ((gmtl.AABoxf) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.AABoxf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AABoxfMarshaler mInstance = new AABoxfMarshaler();
}


} // namespace gmtl
