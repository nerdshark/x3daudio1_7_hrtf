#pragma once

#include "hrtf/HrtfDataSet.h"

#define NOMINMAX
#include <xapobase.h>

#include "math/math_types.h"

#include <vector>
#include <memory>


struct HrtfXapoParam
{
	// sound volume [0; 1]
	float volume_multiplier;

	// elevation, in listener's local space, in radians in the range [-pi/2; +pi/2] where 0 is horizontal direction.
	float elevation;

	// azimuth, in listener's local space, in radians in the range [-2pi; +2pi] where 0 is forward direction.
	float azimuth;

	// distance to the source [0; +inf)
	float distance;
};


class __declspec(uuid("{80D4BED4-7605-4E4C-B29C-5579C375C599}")) HrtfXapoEffect : public CXAPOParametersBase
{
public:
	static HrtfXapoEffect* CreateInstance();

public:
	HrtfXapoEffect(const std::shared_ptr<IHrtfDataSet> & hrtf_data);

	// Inherited via CXAPOParametersBase
	STDMETHOD(LockForProcess)(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters) override;
	STDMETHOD_(void, Process)(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters, BOOL IsEnabled) override;

private:
	bool has_history() const;
	void process_valid_buffer(const float * pInput, float * pOutput, const UINT32 frame_count, const HrtfXapoParam & params);
	void process_invalid_buffer(float * pOutput, const UINT32 frames_to_write_count, UINT32 & valid_frames_counter, const HrtfXapoParam & params);
	void bypass(const float * pInput, float * pOutput, const UINT32 frame_count, const bool is_valid);
	void convolve(const UINT32 frame_index, DirectionData& hrtf_data, float& output);
	void process_frame(float & left_output, float & right_output, const UINT32 frame_index);
	static XAPO_REGISTRATION_PROPERTIES m_regProps;

	WAVEFORMATEX m_input_format;
	WAVEFORMATEX m_output_format;
	HrtfXapoParam m_params[3]; // ring buffer as CXAPOParametersBase requires
	float m_time_per_frame;

	std::shared_ptr<IHrtfDataSet> m_hrtf_data_set;
	const IHrtfData * m_hrtf_data;
	std::vector<float> m_signal;
	UINT32 m_invalid_buffers_count;
	UINT32 m_buffers_per_history;
	UINT32 m_history_size;

	DirectionData m_hrtf_data_left;
	DirectionData m_hrtf_data_right;
};
