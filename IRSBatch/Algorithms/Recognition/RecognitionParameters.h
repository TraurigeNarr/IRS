#pragma once

namespace Algorithms
{

	struct Recognition_Parameters
	{
		// points contain points itself
		std::vector<Vector3D> m_points;

		bool m_show_contours = false;
		// max number of samples with which algo will be trained
		int max_samples_for_symbol = 1000;
		// must be path with folders named as expected symbol
		std::wstring path_to_samples;
		std::wstring classification_file;
		std::wstring images_file;
	};

}