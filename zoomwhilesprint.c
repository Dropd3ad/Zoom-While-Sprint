modded class DayZPlayerImplement
{
	override 	void HandleView()
	{
		if (!IsAlive())
		{
			if (m_CameraIronsight || m_CameraOptics)
			{
				ExitSights();
			}
			return;
		}
		
		HumanInputController hic = GetInputController();
	
		bool camChange = hic.CameraViewChanged();
		
		if (IsRaised())
		{
			if (m_IsWeapon)
			{
				camChange = false;
			}
		}
		
		//! 3rd person camera
		if (GetGame().GetWorld().Is3rdPersonDisabled())
		{
			m_Camera3rdPerson = false;
		}
		else if (camChange)
		{
			m_Camera3rdPerson = !m_Camera3rdPerson;
		}
		
		// exits optics completely, comment to return to ADS
		if ( m_LiftWeapon_player && (m_CameraOptics|| m_CameraIronsight) )
			ExitSights();
		
		if (IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE) && (m_CameraOptics || m_CameraIronsight))
			GetDayZPlayerType().SetVerticalMinimumAimLimit(DayZPlayerCameraIronsights.CONST_UD_MIN_BACK);
		else
			GetDayZPlayerType().SetVerticalMinimumAimLimit(DayZPlayerCameraIronsights.CONST_UD_MIN);
		
		if ( !m_MovementState.IsRaised() && hic.IsZoomToggle() )        
		{
			if (m_MovementState.m_iMovement == 3)
			{
				m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
			}
			else
			{
				m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
			}
		}
		else
		{
			m_CameraEyeZoomLevel = ECameraZoomType.NONE;
		}
		
		if (m_MovementState.IsRaisedInProne())
		{
			float headingAngle = MiscGameplayFunctions.GetHeadingAngle(this);
			float headingAngleDiffBlock = Math.AbsFloat(m_LastHeadingAngleBlock - headingAngle);
			float headingAngleDiffBlock2 = Math.AbsFloat(m_LastHeadingAngleBlock2 - headingAngle);
			float headingAngleDiff = 0.0;
			
			if (m_LastHeadingAngle * headingAngle < 0.0)
			{
				headingAngleDiff = Math.AbsFloat(m_LastHeadingAngle + headingAngle);
			}
			else
			{
				headingAngleDiff = Math.AbsFloat(m_LastHeadingAngle - headingAngle);
			}
			
			if (headingAngleDiff > 0.2)
			{
				int time = GetGame().GetTime();
				
				float timefilterconstant = 400 - (headingAngleDiff * 100);
				
				if (headingAngleDiffBlock > 0.8 && time - m_LastBackSoundTime > timefilterconstant)
				{
					float volume = headingAngleDiff / 0.5;
					if (volume > 1)
					{
						volume = 1;
					}
					if (volume < 0.25)
					{
						volume = 0.25;
					}

					m_SoundOffset = headingAngleDiff / 2;
					if (m_SoundOffset > 0.25)
					{
						m_SoundOffset = 0.25;
					}

					string soundSetName = "Cloth_Body_longmove_TShirt_Soundset";
					
					string bodyClothName = "";
					EntityAI attachment = GetInventory().FindAttachment(InventorySlots.BODY);
					if (attachment)
					{
						bodyClothName = attachment.GetAttachmentSoundType();
					}

					if (bodyClothName != "")
					{
						string path = "CfgSoundTables CfgAttachmentSoundTables Cloth_Body_Longmove_LookupTable";
						int soundCount = GetGame().ConfigGetChildrenCount(path);

						for (int i = 0; i < soundCount; i++)
						{
							string name = "";
							GetGame().ConfigGetChildName(path, i, name);
							
							if (name == bodyClothName)
							{
								TStringArray stringArray = new TStringArray();
								GetGame().ConfigGetTextArray(path + " " + name + " soundSets", stringArray);
								soundSetName = stringArray.Get(0);

								delete stringArray;
							}
						}
					}

					SoundParams soundParams = new SoundParams(soundSetName);
					SoundObjectBuilder soundObjectBuilder = new SoundObjectBuilder(soundParams);
					
					if (soundObjectBuilder != NULL)
					{
						SoundObject soundObject = soundObjectBuilder.BuildSoundObject();
						
						if (soundObject != NULL)
						{
							soundObject.SetPosition(GetPosition());

							AbstractWave wave = GetGame().GetSoundScene().Play3D(soundObject, soundObjectBuilder);
							wave.SetStartOffset(m_SoundOffset);
							wave.SetVolumeRelative(volume);

							m_LastBackSoundTime = GetGame().GetTime();
							m_LastHeadingAngleBlock = headingAngle;
						}
					}
				}

				if (headingAngleDiffBlock2 > 1.5 && time - m_LastBackSoundTime2 > (timefilterconstant * 2))
				{
					float volume2 = headingAngleDiff * 2;
					if (volume2 > 1)
					{
						volume2 = 1;
					}

					m_SoundOffset = headingAngleDiff / 1.8;
					if (m_SoundOffset < 0.1)
					{
						m_SoundOffset = 0.1;
					}
					if (m_SoundOffset > 0.3)
					{
						m_SoundOffset = 0.3;
					}

					string soundSetName2 = "walkProne_noHS_asphalt_ext_Char_SoundSet";
					string surfaceType = GetSurfaceType();

					if (surfaceType != "")
					{
						string movementSurfaceType = "walkProne_" + surfaceType;
						
						string path2 = "CfgSoundTables CfgStepSoundTables walkProne_noHS_Char_LookupTable";
						int soundCount2 = GetGame().ConfigGetChildrenCount(path2);

						for (int i2 = 0; i2 < soundCount2; i2++)
						{
							string name2 = "";
							GetGame().ConfigGetChildName(path2, i2, name2);
							
							if (name2 == movementSurfaceType)
							{
								TStringArray stringArray2 = new TStringArray();
								GetGame().ConfigGetTextArray(path2 + " " + name2 + " soundSets", stringArray2);
								soundSetName2 = stringArray2.Get(0);

								delete stringArray2;
							}
						}
					}

					SoundParams soundParams2 = new SoundParams(soundSetName2);
					SoundObjectBuilder soundObjectBuilder2 = new SoundObjectBuilder(soundParams2);
					
					if (soundObjectBuilder2 != NULL)
					{
						SoundObject soundObject2 = soundObjectBuilder2.BuildSoundObject();
						
						if (soundObject2 != NULL)
						{
							soundObject2.SetPosition(GetPosition());

							AbstractWave wave2 = GetGame().GetSoundScene().Play3D(soundObject2, soundObjectBuilder2);
							wave2.SetStartOffset(m_SoundOffset);
							wave2.SetVolumeRelative(volume2);            

							m_LastBackSoundTime2 = GetGame().GetTime();
							m_LastHeadingAngleBlock2 = headingAngle;
						}
					}
				}

				m_LastHeadingAngle = headingAngle;
			}
		}
	}
}