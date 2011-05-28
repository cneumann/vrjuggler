/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_INPUT_DEVICE_H_
#define _GADGET_INPUT_DEVICE_H_

#include <gadget/gadgetConfig.h>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <gadget/Type/AllBaseTypes.h>


namespace gadget
{

/**
 * @example "Example use of gadget::InputDevice"
 *
 * The code below shows how to set the base class for a device that uses
 * digital, analog, and positonal data. The order of the types listed in
 * boost::mpl::inherit does not matter.
 *
 * \code
 * #include <boost/mpl/inherit.hpp>
 * #include <gadget/Type/InputDevice.h>
 *
 * typedef
 *    boost::mpl::inherit<
 *         gadget::Digital
 *       , gadget::Analog
 *       , gadget::Position
 *    >::type
 * base_input_type;
 *
 * class MyDriver
 *    : public gadget::InputDevice<base_input_type>
 * {
 * };
 * \endcode
 *
 * Do not include gadget::Input in the type list. It is already a base class
 * of gadget::InputDevice.
 *
 * For the case of a device that uses only one type of data (such as analog),
 * the usage is even simpler:
 *
 * \code
 * #include <gadget/Type/InputDevice.h>
 *
 * class MyDriver
 *    : public gadget::InputDevice<gadget::Analog>
 * {
 * };
 * \endcode
 */

/** \class InputDevice InputDevice.h gadget/Type/InputDevice.h
 *
 * @since 2.1.18
 */
template<typename Base>
class InputDevice
   : public Input
   , public Base
{
public:
   /** @name vpr::SerializableObject Overrides */
   //@{
   virtual void writeObject(vpr::ObjectWriter* writer)
   {
      Input::writeObject(writer);
      Caller<WriteInvoker> caller(this, writer);
      boost::mpl::for_each<all_base_types, wrap<boost::mpl::_1> >(caller);
   }

   virtual void readObject(vpr::ObjectReader* reader)
   {
      Input::readObject(reader);
      Caller<ReadInvoker> caller(this, reader);
      boost::mpl::for_each<all_base_types, wrap<boost::mpl::_1> >(caller);
   }
   //@}

   /** @name gadget::Input Overrides */
   //@{
   virtual std::string getInputTypeName()
   {
      std::string result(Input::getInputTypeName());
      Caller<NameBuilder> builder(this, &result);
      boost::mpl::for_each<all_base_types, wrap<boost::mpl::_1> >(builder);
      return result;
   }
   //@}

private:
   /**
    * This exists solely to allow abstract Gadgeteer device types (such as
    * gadget::Input) to be used in the type sequence that is used for
    * iteration in the call to boost::mpl::for_each<>().
    */
   template<typename T>
   struct wrap
   {
#if defined(__GNUC__) && __GNUC__ < 4
      /**
       * This type exists to fix compile errors with versions of GCC older
       * than 4.0. It is used internally by MPL when wrap<T> is utilized by
       * boost::mpl::for_each().
       */
      template<typename U>
      struct apply
      {
         typedef wrap<U> type;
      };
#endif
   };

   struct NoOp
   {
      template<typename B, typename T1, typename T2>
      static void invoke(const T1&, const T2&)
      {}
   };

   struct WriteInvoker
   {
      typedef vpr::ObjectWriter* arg_type;

      template<typename BaseType>
      static void invoke(BaseType* obj, vpr::ObjectWriter* writer)
      {
         obj->BaseType::writeObject(writer);
      }
   };

   struct ReadInvoker
   {
      typedef vpr::ObjectReader* arg_type;

      template<typename BaseType>
      static void invoke(BaseType* obj, vpr::ObjectReader* reader)
      {
         obj->BaseType::readObject(reader);
      }
   };

   struct NameBuilder
   {
      typedef std::string* arg_type;

      template<typename BaseType>
      static void invoke(BaseType* obj, std::string* result)
      {
         *result += obj->BaseType::getInputTypeName();
      }
   };

   template<typename InvokerType
          , typename ArgType = typename InvokerType::arg_type
          >
   class Caller
   {
   public:
      Caller(InputDevice* owner, ArgType arg)
         : owner(owner)
         , arg(arg)
      {
      }

      template<typename BaseType>
      void operator()(wrap<BaseType>)
      {
         typedef typename
            boost::mpl::if_<
                 boost::is_base_of<BaseType, InputDevice>
               , InvokerType
               , NoOp
            >::type
         invoker_type;

         invoker_type::template invoke<BaseType>(owner, arg);
      }

   private:
      InputDevice* owner;
      ArgType arg;
   };
};

}


#endif /* _GADGET_INPUT_DEVICE_H_ */