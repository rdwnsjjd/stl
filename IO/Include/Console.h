// Copyright (C) 2022 Emad
// 
// This file is part of sandikCpp.
// 
// sandikCpp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// sandikCpp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with sandikCpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _FRAMEWORK_STL_IO_INCLUDE_CONSOLE_H_
#define _FRAMEWORK_STL_IO_INCLUDE_CONSOLE_H_

#include <stdio.h>

#include "Framework/Basics/Inc"
#include "Stream.h"

using namespace framework::basics;

namespace stl { namespace io {

    class Console {
      private:

        inline Console(const Console&&) { 
            // this->_stdin  = Stream(STDIN_FILENO);
            // this->_stdout = Stream(STDOUT_FILENO);
            // this->_stderr = Stream(STDERR_FILENO);
        }
        

    };
    
} // namespace io

} // namespace stl 


#endif // _FRAMEWORK_STL_IO_INCLUDE_CONSOLE_H_
