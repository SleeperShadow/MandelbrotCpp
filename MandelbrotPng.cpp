/* 
 * Mandlebrot image using Tiny PNG Output (C++)
 * 
 * Copyright (c) 2018 Project Nayuki
 * https://www.nayuki.io/page/tiny-png-output
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program (see COPYING.txt and COPYING.LESSER.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "Params.h"
#include "PieceManager.h"

int main(int argc, char** argv) {
	
	ImageParams params;
	params.initParameters(argc, argv);

	int exitStatus = 0;

	if (params.quietMode)
	{
		PieceManager<false> manager(params);
		exitStatus = manager.run();
	}
	else
	{
		PieceManager<true> loggingManager(params);
		exitStatus = loggingManager.run();
	}
	return exitStatus;
}
