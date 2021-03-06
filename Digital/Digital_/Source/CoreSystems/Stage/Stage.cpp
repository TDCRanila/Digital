#include <CoreSystems/Stage/Stage.h>

#include <CoreSystems/Logging/Logger.h>

namespace DCore
{

	StageBase::StageBase(std::string a_stage_name, bool a_start_disabled)
		: _name(a_stage_name)
		, _id(DUIDGenerator::GenerateID())
		, _is_disabled(a_start_disabled)
	{
	}

	void StageBase::RenderImGui()
	{

	}

	bool StageBase::IsDisabled() const
	{
		return _is_disabled;
	}

	void StageBase::OnEnable()
	{

	}

	void StageBase::OnDisable()
	{

	}
	
	void StageBase::OnApplicationEvent(ApplicationEvent& a_event)
	{

	}

	void StageBase::OnStageEvent(StageEvent& a_event)
	{

	}
	
	StageID StageBase::GetID() const
	{
		return _id;
	}

	std::string StageBase::GetName() const
	{
		return _name;
	}

	void StageBase::Enable()
	{
		if (_is_disabled)
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - being enabled.", _id, _name);
			_is_disabled = false;
			OnEnable();

			StageEnabledEvent event(_id, _name, _is_disabled);
			RequestEventBroadcast(event);
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already enabled.", _id, _name);
		}
	}

	void StageBase::Disable()
	{
		if (!_is_disabled)
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - being disabled.", _id, _name);
			_is_disabled = true;
			OnDisable();

			StageDisabledEvent event(_id, _name, _is_disabled);
			RequestEventBroadcast(event);
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already disabled.", _id, _name);
		}
	}

	void StageBase::SetStageStackCommunicator(std::shared_ptr<StageStackCommunicator> a_communicator)
	{
		_stage_stack_communicator = a_communicator;
	}

	void StageBase::RequestEventBroadcast(StageEvent& a_event)
	{
		_stage_event_callback_func(a_event);
	}

	std::shared_ptr<StageStackCommunicator> StageBase::GetStageStackCommunicator() const
	{
		return _stage_stack_communicator;
	}

	void StageBase::BindStageEventFunc(const StageEventCallbackFunc& a_event_callback_func)
	{
		_stage_event_callback_func = a_event_callback_func;
	}

} // End of namespace ~ DCore.
