#include "Scripter.h"
#include "processPrep.h"

bool Scripter::Parse()
{
	for(vector<Script>::iterator i=pnow->begin();i!=pnow->end();i++)
	{
		bool rv = false;
		int depth = 0;
		if(!i->value)
			continue;

		//Filter Type
		DWORD nowval = i->value;
		switch (nowval & SCRKWMASK_TYPE)
		{
			/************************************************************************/
			/* Action                                                               */
			/************************************************************************/
		case SCRKW_ACTION:
			switch (nowval)
			{
			case SCR_SD:
				if(rv = Copy(&i, 2))
				{
					int _tdi = CAST(d[0]);
					CINT(d[_tdi].value) = CAST(d[1]);
					d[_tdi].bfloat = false;
				}
				break;
			case SCR_SDF:
				if(rv = Copy(&i, 2))
				{
					int _tdi = CAST(d[0]);
					CFLOAT(d[_tdi].value) = CAST(d[1]);
					d[_tdi].bfloat = true;
				}
				break;
			case SCR_SDU:
				if(rv = Copy(&i, 2))
				{
					int _tdi = CAST(d[0]);
					CUINT(d[_tdi].value) = CUINT(d[1].value);
					d[_tdi].bfloat = false;
				}
				break;
			case SCR_ST:
				if(rv = Copy(&i, 3))
				{
					int _tdi = CAST(d[0]);
					tar[_tdi].x = CAST(d[1]);
					tar[_tdi].y = CAST(d[2]);
				}
				break;
			case SCR_CALL:
				if(rv = Copy(&i, 2))
				{
					vector<Script> * psaved = pnow;
					functionExecute(UCAST(d[0]), UCAST(d[1]));
					pnow = psaved;
				}
				break;
			case SCR_EXECUTE:
				if(rv = Copy(&i, 3))
				{
					vector<Script> * psaved = pnow;
					Execute(UCAST(d[0]) + SCRIPT_CTOFFSET, UCAST(d[1]), UCAST(d[2]));
					pnow = psaved;
				}
				break;

			case SCR_RETURN:
				if(rv = Copy(&i, 1))
				{
					mp.retvalue = CAST(d[0]);
					return true;
				}
				break;

			case SCR_SETSTATE:
				if(rv = Copy(&i, 1))
				{
					mp.state = CAST(d[0]);
				}
				break;
			case SCR_SETTIME:
				if(rv = Copy(&i, 1))
				{
					time = CAST(d[0]);
				}
				break;
			case SCR_SETDIFFI:
				if(rv = Copy(&i, 1))
				{
					mp.nowdifflv = CAST(d[0]);
					if(mp.nowdifflv < 3)
						mp.defaultdifflv = mp.nowdifflv;
				}
				break;
			case SCR_SETCHARA:
				if(rv = Copy(&i, 1))
				{
					mp.luchara = CAST(d[0]);
					Player::p.ID = mp.luchara << 1;
				}
				break;
			case SCR_SETSCENE:
				if(rv = Copy(&i, 1))
				{
					mp.scene = CAST(d[0]);
				}
				break;
			case SCR_SETMODE:
				if(rv = Copy(&i, 1))
				{
					int _tdi = CAST(d[0]);
					mp.rangemode = (bool)(_tdi & RPYMODE_RANGE);
					mp.practicemode = (bool)(_tdi & RPYMODE_PRACTICE);
				}
				break;

			case SCR_SETKEY:
				if (rv = Copy(&i, 2))
				{
					hge->Input_SetDIKey(CAST(d[0]), CAST(d[1]));
				}
				break;
			case SCR_DISABLEALLKEY:
				if (rv = true)
				{
					hge->Input_SetDIKey(KS_FIRE_MP, false);
					hge->Input_SetDIKey(KS_BOMB_MP, false);
					hge->Input_SetDIKey(KS_CIRCLE_MP, false);
					hge->Input_SetDIKey(KS_SLOW_MP, false);
					hge->Input_SetDIKey(KS_UP_MP, false);
					hge->Input_SetDIKey(KS_DOWN_MP, false);
					hge->Input_SetDIKey(KS_LEFT_MP, false);
					hge->Input_SetDIKey(KS_RIGHT_MP, false);
				}
				break;

			case SCR_MUSICCHANGE:
				if(rv = Copy(&i, 2))
				{
					mp.musicChange(CAST(d[0]), (bool)(CAST(d[1])));
				}
				break;
			case SCR_MUSICSLIDE:
				if (rv = Copy(&i, 2))
				{
					mp.musicSlide(CAST(d[0]), CAST(d[1]));
				}

			case SCR_SE:
				if(rv = Copy(&i, 2))
				{
					SE::push(CAST(d[0]), CAST(d[1]));
				}
				break;
			case SCR_SEOFF:
				if (rv = Copy(&i, 1))
				{
					SE::stop(CAST(d[0]));
				}

			case SCR_EFFSETUP:
				if (rv = Copy(&i, 5))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
				}
				break;
			case SCR_EFFSETUPEX:
				if(rv = Copy(&i, 10))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
				}
				break;
			case SCR_EFFSETUPCHASE:
				if (rv = Copy(&i, 8))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]));
				}
				break;
			case SCR_EFFSTOP:
				if (rv = Copy(&i, 1))
				{
					int _tdi = CAST(d[0]);
					if (effsys[_tdi].exist && effsys[_tdi].eff)
					{
						effsys[_tdi].Stop();
					}
				}
				break;
			case SCR_EFFOFF:
				if(rv = Copy(&i, 1))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].exist = false;
				}
				break;
			case SCR_EFFSET:
				if(rv = Copy(&i, 4))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].angle = CAST(d[1]);
					effsys[_tdi].speed = CAST(d[2]);
					effsys[_tdi].zSpeed = CAST(d[3]);
				}
				break;
			case SCR_EFFMOVETO:
				if(rv = Copy(&i, 4))
				{
					int _tdi = CAST(d[0]);
					effsys[_tdi].x = CAST(d[1]);
					effsys[_tdi].y = CAST(d[2]);
					effsys[_tdi].z = CAST(d[3]);
				}
				break;

			case SCR_BOSSSTORE:
				if (rv = true)
				{
					Enemy::bossflag[en[Enemy::index].type - ENEMY_BOSSTYPEBEGIN] = BOSS_STORE;
				}
				break;
			case SCR_BOSSATTACK:
				if (rv = true)
				{
					Enemy::bossflag[en[Enemy::index].type - ENEMY_BOSSTYPEBEGIN] = BOSS_ATTACK;
				}
				break;
			}
			break;
			/************************************************************************/
			/* Basic                                                                */
			/************************************************************************/
		case SCRKW_BASIC:
			switch (nowval & SCRKWMASK_CLASS)
			{
				/************************************************************************/
				/* AttackObject                                                         */
				/************************************************************************/
			case SCRKW_CLASS_ATTACKOBJECT:
				switch (nowval)
				{
				case SCR_BUBUILD:
					if(rv = Copy(&i, 10))
					{
						Bullet::Build(CAST(d[0]), CAST(d[1]), (bool)(CAST(d[2])), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
					}
					break;
				case SCR_BUACTIONSET:
					if(rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if(!_tdi)
						{
							Bullet::_bu.actionList[0] = SECTIONEND;
						}
						else if(rv = Copy(&i, _tdi))
						{
							for(int j=0;j<_tdi;j++)
							{
								Bullet::_bu.actionList[j] = CAST(d[j]);
							}
							Bullet::_bu.actionList[_tdi] = SECTIONEND;
						}
					}
					break;
				case SCR_BUBUILDCIRCLE:
					if (rv = Copy(&i, 9))
					{
						int _tnum = CAST(d[0]);
						int _tanglebase;
						int _tanglestep = 36000 / _tnum;

						float _tx = CAST(d[2]);
						float _ty = CAST(d[3]);
						float _tspeed = CAST(d[4]);
						BYTE _ttype = CAST(d[5]);
						BYTE _tcolor = CAST(d[6]);
						int _tfadeintime = CAST(d[7]);
						float _tavoid = CAST(d[8]);
						if (CAST(d[1]))
						{
							_tanglebase = RANDA;
						}
						else
						{
							BObject _tobj;
							_tobj.x = _tx;
							_tobj.y = _ty;
							_tanglebase = _tobj.aMainAngle(Player::p);
						}
						for (int i=0; i<_tnum; i++)
						{
							Bullet::Build(_tx, _ty, true, _tanglebase + i * _tanglestep, _tspeed, _ttype, _tcolor, _tfadeintime, _tavoid, 0xff);
						}
					}
					break;
				case SCR_BUDECANCEL:
					if (rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi >= 0 && _tdi < BULLETMAX)
						{
							if (bu.toIndex(_tdi) != VECLST_INDEXERROR)
							{
								(*bu).cancelable = false;
							}
						}
						break;

					}
					break;

				case SCR_BEBUILD:
					if(rv = Copy(&i, 11))
					{
						Beam::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), (bool)(CAST(d[7])), (bool)(CAST(d[8])), CAST(d[9]), CAST(d[10]));
					}
					break;
				case SCR_BEVECTOR:
					if (rv = Copy(&i, 5))
					{
						int _tdi = CAST(d[0]);
						if (be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (be.isInRange() && be.isValid())
							{
								if ((*be).able)
								{
									(*be).SetVector(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
								}
							}
						}
					}
					break;
				case SCR_BEHOLD:
					if (rv = Copy(&i, 3))
					{
						int _tdi = CAST(d[0]);
						if (be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (be.isInRange() && be.isValid())
							{
								if ((*be).able)
								{
									(*be).SetHold(CAST(d[1]));
									(*be).angle = CAST(d[2]);
								}
							}
						}
					}
					break;
				case SCR_BEPIN:
					if (rv = Copy(&i, 3))
					{
						int _tdi = CAST(d[0]);
						if (be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (be.isInRange() && be.isValid())
							{
								if ((*be).able)
								{
									(*be).SetHold(CAST(d[1]), CAST(d[2]));
								}
							
							}
						}
					}
					break;
				}
				break;
				/************************************************************************/
				/* Ghost                                                                */
				/************************************************************************/
			case SCRKW_CLASS_GHOST:
				switch (nowval)
				{
				case SCR_GHBUILD:
					if(rv = Copy(&i, 10))
					{
						Ghost::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
					}
					break;
				case SCR_GHSAIM:
					if(rv = Copy(&i, 2))
					{
						gh[Ghost::index].aim.x = CAST(d[0]);
						gh[Ghost::index].aim.y = CAST(d[1]);
					}
					break;
				case SCR_GHSET:
					if(rv = Copy(&i, 3))
					{
						gh[Ghost::index].speed = CAST(d[0]);
						gh[Ghost::index].angle = CAST(d[1]);
						gh[Ghost::index].ac = CAST(d[2]);
					}
					break;
				case SCR_GHCHASE:
					if (rv = Copy(&i, 3))
					{
						gh[Ghost::index].chaseAim(CAST(d[0]), CAST(d[1]), CAST(d[2]));
					}
					break;
				}
				break;
				/************************************************************************/
				/* Enemy                                                                */
				/************************************************************************/
			case SCRKW_CLASS_ENEMY:
				switch (nowval)
				{
				case SCR_ENTEXASSIGN:
					if(rv = Copy(&i, ENEMY_TEXMAX))
					{
						int _tdi[ENEMY_TEXMAX];
						for(int j=0; j<ENEMY_TEXMAX; j++)
						{
							_tdi[j] = CAST(d[j]);
						}
						Enemy::texAssign(mp.tex[_tdi[0]], mp.tex[_tdi[1]], mp.tex[_tdi[2]], mp.tex[_tdi[3]]);
					}
					break;
				case SCR_ENBUILD:
					if(rv = Copy(&i, 10))
					{
						Enemy::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), UCAST(d[9]));
					}
					break;
				case SCR_ENACTIONSET:
					if(rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if(rv = Copy(&i, _tdi))
						{
							if(_tdi)
							{
								en[Enemy::index].ac = CAST(d[0]);
								for(int j=0;j<_tdi-1;j++)
								{
									en[Enemy::index].para[j] = CAST(d[j+1]);
								}
							}
						}
					}
					break;
				case SCR_ENSAIM:
					if(rv = Copy(&i, 2))
					{
						en[Enemy::index].aim.x = CAST(d[0]);
						en[Enemy::index].aim.y = CAST(d[1]);
					}
					break;
				case SCR_ENCHASE:
					if (rv = Copy(&i, 3))
					{
						en[Enemy::index].chaseAim(CAST(d[0]), CAST(d[1]), CAST(d[2]));
					}
					break;
				}
				break;
				/************************************************************************/
				/* Math                                                                 */
				/************************************************************************/
			case SCRKW_CLASS_MATH:
				switch (nowval)
				{
				case SCR_RAMA:
					if(rv = Copy(&i, 7))
					{
						int _tdi = CAST(d[6]);
						BObject _tobj;
						_tobj.x = CAST(d[1]);
						_tobj.y = CAST(d[2]);
						if(CAST(d[0]))
						{
							CINT(d[_tdi].value) = _tobj.rMainAngle(CAST(d[3]), CAST(d[4]), CAST(d[5]));
						}
						else
						{
							CINT(d[_tdi].value) = _tobj.aMainAngle(CAST(d[3]), CAST(d[4]), CAST(d[5]));
						}
						d[_tdi].bfloat = false;
					}
					break;
				case SCR_DIST:
					if (rv = Copy(&i, 5))
					{
						int _tdi = CAST(d[4]);
						float _txo = CAST(d[0]);
						float _tyo = CAST(d[1]);
						float _txa = CAST(d[2]);
						float _tya = CAST(d[3]);
						CFLOAT(d[_tdi].value) = DIST(_txo, _tyo, _txa, _tya);
						d[_tdi].bfloat = true;
					}
					break;
				}
				break;
				/************************************************************************/
				/* Other                                                                */
				/************************************************************************/
			case SCRKW_CLASS_OTHER:
				switch (nowval)
				{
				}
				break;
				/************************************************************************/
				/* Special                                                              */
				/************************************************************************/
			case SCRKW_CLASS_SPECIAL:
				switch (nowval)
				{
				case SCR_BOSSUP:
					if(rv = true)
					{
						bossinfo.bossUp();
					}
					break;
				case SCR_RANGEUP:
					if(rv = Copy(&i, 3))
					{
						if(mp.rangemode)
						{
							int _tdi = CAST(d[0]);
							tar[_tdi].x = CAST(d[1]);
							tar[_tdi].y = CAST(d[2]);
						}
					}
					break;

				case SCR_ACONTROL:
					if (rv = Copy(&i, 2))
					{
						int _tindex = CAST(d[0]);
						if (_tindex >= 0 && _tindex < BULLETMAX)
						{
							if (bu.toIndex(_tindex) != VECLST_INDEXERROR)
							{
								if (bu.isInRange() && bu.isValid())
								{
									if ((*bu).able)
									{
										int _tdi = CAST(d[1]);
										if(!_tdi)
										{
											(*bu).actionList[0] = SECTIONEND;
										}
										else if(rv = Copy(&i, _tdi))
										{
											for(int j=0;j<_tdi;j++)
											{
												(*bu).actionList[j] = CAST(d[j]);
											}
											(*bu).actionList[_tdi] = SECTIONEND;
										}
									}
								}
							}
						}
					}
					break;
				case SCR_BCONTROL:
					if(rv = Copy(&i, 7))
					{
						BObject * _tobj = NULL;
						int _ttype = CAST(d[0]);
						int _tdi = CAST(d[1]);
						int _tindex = CAST(d[_tdi]);

						if(_tindex == BINDEX_NULL)
							break;

						switch(_ttype)
						{
						case BTYPE_BULLET:
							if (_tindex >= 0 && _tindex < BULLETMAX)
							{
								if (bu.toIndex(_tindex) != VECLST_INDEXERROR)
								{
									if (bu.isInRange() && bu.isValid())
									{
										if ((*bu).able)
										{
											_tobj = &(*bu);
										}
									}
								}
							}
							break;
						case BTYPE_BEAM:
							if (_tindex >= 0 && _tindex < BEAMMAX)
							{
								if (be.toIndex(_tindex) != VECLST_INDEXERROR)
								{
									if (be.isInRange() && be.isValid())
									{
										if ((*be).able)
										{
											_tobj = &(*be);
										}
									}
								}
							}
							break;
						case BTYPE_ENEMY:
							if(en[_tindex].able)
								_tobj = &en[_tindex];
							break;
						case BTYPE_GHOST:
							if(gh[_tindex].able)
								_tobj = &gh[_tindex];
							break;
						}

						if(_tobj)
						{
							_tobj->x = CAST(d[2]);
							_tobj->y = CAST(d[3]);
							_tobj->angle = CAST(d[4]);
							_tobj->headangle = CAST(d[5]);
							_tobj->speed = CAST(d[6]);
						}
						else
						{
							CINT(d[_tdi].value) = BINDEX_NULL;
							d[_tdi].bfloat = false;
						}
					}
					break;

				case SCR_COLLISION_CIRCLE:
					if (rv = Copy(&i, 6))
					{
						int _tdi = CAST(d[5]);
						BObject _tobj;
						_tobj.x = CAST(d[0]);
						_tobj.y = CAST(d[1]);
						CINT(d[_tdi].value) = _tobj.checkCollisionCircle(CAST(d[2]), CAST(d[3]), CAST(d[4]));
						d[_tdi].bfloat = false;
					}
					break;
				case SCR_COLLISION_SQUARE:
					if (rv = Copy(&i, 6))
					{
						int _tdi = CAST(d[5]);
						BObject _tobj;
						_tobj.x = CAST(d[0]);
						_tobj.y = CAST(d[1]);
						CINT(d[_tdi].value) = _tobj.checkCollisionSquare(CAST(d[2]), CAST(d[3]), CAST(d[4]));
						d[_tdi].bfloat = false;
					}
					break;
				}
				break;
				/************************************************************************/
				/* Effect                                                               */
				/************************************************************************/
			case SCRKW_CLASS_EFFECT:
				switch (nowval)
				{
				}
				break;
				/************************************************************************/
				/* Player                                                               */
				/************************************************************************/
			case SCRKW_CLASS_PLAYER:
				switch (nowval)
				{
				case SCR_SETPPOS:
					if (rv = Copy(&i, 2))
					{
						float _tdfx = CAST(d[0]);
						float _tdfy = CAST(d[1]);
						_tdfx = _tdfx < PL_MOVABLE_LEFT ? PL_MOVABLE_LEFT : _tdfx;
						_tdfx = _tdfx > PL_MOVABLE_RIGHT ? PL_MOVABLE_RIGHT : _tdfx;
						_tdfy = _tdfy < PL_MOVABLE_TOP ? PL_MOVABLE_TOP : _tdfy;
						_tdfy = _tdfy > PL_MOVABLE_BOTTOM ? PL_MOVABLE_BOTTOM : _tdfy;
						Player::p.x = _tdfx;
						Player::p.y = _tdfy;
					}
					break;
				case SCR_SETPLIFE:
					if (rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nLife = _tdi > M_PL_PLAYERMAX ? M_PL_PLAYERMAX : _tdi;
					}
					break;
				case SCR_SETPBOMB:
					if (rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nBomb = _tdi > M_PL_BOMBMAX ? M_PL_BOMBMAX : _tdi;
					}
					break;
				case SCR_SETPPOWER:
					if(rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nPower = _tdi;
						if(Player::p.nPower >= PL_POWERMAX)
						{
							Player::p.nPower = PL_POWERMAX;
							Player::p.bFull = true;
						}
						else
						{
							Player::p.bFull = false;
						}
					}
					break;
				case SCR_SETPALIVENESS:
					if (rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nAliveness = _tdi;
					}
					break;
				case SCR_SETPPOINT:
					if (rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nPoint = _tdi;
					}
					break;
				case SCR_SETPBCIRCLE:
					if (rv = Copy(&i, 1))
					{
						if ((bool)(CAST(d[0])) ^ Player::p.bCircle)
						{
							Player::p.callCircleUncircleChange();
						}
					}
					break;
				case SCR_SETPBSLOW:
					if (rv = Copy(&i, 1))
					{
						Player::p.callSlowFastChange((bool)(CAST(d[0])));
					}
					break;
				case SCR_SETPBINFI:
					if(rv = true)
					{
						Player::p.bInfi = true;
					}
					break;
				case SCR_SETPSPEED:
					if (rv = Copy(&i, 1))
					{
						Player::p.speed = CAST(d[0]);
					}
					break;
				case SCR_SETPSLOWSPEED:
					if (rv = Copy(&i, 1))
					{
						Player::p.slowspeed = CAST(d[0]);
					}
					break;

				case SCR_COLLAPSE:
					if (rv = true)
					{
						Player::p.callCollapse();
					}
					break;
				}
				break;
				/************************************************************************/
				/* Layer                                                                */
				/************************************************************************/
			case SCRKW_CLASS_LAYER:
				switch (nowval)
				{
				case SCR_BGVALUE:
					if(rv = Copy(&i, 11))
					{
						int _tdi = CAST(d[0]);
						int _tdtexi = CAST(d[1]);
						ubg[_tdi]->valueSet(mp.tex[_tdtexi], CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), UCAST(d[10]));
					}
					break;
				case SCR_BGVALEX:
					if (rv = Copy(&i, 20))
					{
						int _tdi = CAST(d[0]);
						int _tdtexi = CAST(d[1]);
						ubg[_tdi]->valueSet(mp.tex[_tdtexi], CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]), CAST(d[12]), CAST(d[13]), CAST(d[14]), CAST(d[15]), CAST(d[16]), (bool)(CAST(d[17])), (bool)(CAST(d[18])), UCAST(d[19]));
					}
					break;
				case SCR_BGTEXRECT:
					if(rv = Copy(&i, 5))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->texRectSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
					}
					break;
				case SCR_BGRECT:
					if(rv = Copy(&i, 9))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->rectSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]));
					}
					break;
				case SCR_BGZ:
					if(rv = Copy(&i, 5))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->zSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
					}
					break;
				case SCR_BGSCALE:
					if(rv = Copy(&i, 3))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->scaleSet(CAST(d[1]), CAST(d[2]));
					}
					break;
				case SCR_BGCOLOR:
					if(rv = Copy(&i, 5))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->colorSet(UCAST(d[1]), UCAST(d[2]), UCAST(d[3]), UCAST(d[4]));
					}
					break;
				case SCR_BGMOVE:
					if(rv = Copy(&i, 3))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->moveSet((bool)(CAST(d[1])), (bool)(CAST(d[2])));
					}
					break;
				case SCR_BGFLAG:
					if(rv = Copy(&i, 2))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->flag = CAST(d[1]);
						ubg[_tdi]->changetimer = 0;
					}
					break;
				case SCR_BGPARAL:
					if(rv = Copy(&i, 2))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->parallelogram(CAST(d[1]));
					}
					break;
				case SCR_BG4V:
					if (rv = Copy(&i, 13))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->vertexSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]), CAST(d[12]));
					}
					break;
				case SCR_BGOFF:
					if(rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						ubg[_tdi]->exist = false;
					}
					break;
				case SCR_BGSETUP:
					if(rv = Copy(&i, 3))
					{
						int _tdi = CAST(d[0]);
						if(CAST(d[2]) || BGLayer::set[_tdi].sID == BGLAYERSET_NONE)
						{
							int _tsID = CAST(d[1]);
							if(BGLayer::set[_tdi].sID != BGLAYERSET_NONE || BGLayer::set[_tdi].sID != CAST(d[1]))
							{
								vector<Script> * psaved = pnow;
								scr.sceneExecute(BGLayer::set[_tdi].sID, 0);
								pnow = psaved;
							}
							BGLayer::set[_tdi].sID = _tsID;
							BGLayer::set[_tdi].timer = 0;
						}
					}
					break;
				}
				break;
				/************************************************************************/
				/* Select                                                               */
				/************************************************************************/
			case SCRKW_CLASS_SELECT:
				switch (nowval)
				{
				case SCR_SELBUILD:
					if(rv = Copy(&i, 21))
					{
						int _tdtexi = CAST(d[1]);
						Selector::Build(CAST(d[0]), mp.tex[_tdtexi], CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]),
							CAST(d[11]), CAST(d[13]),
							CAST(d[13]), CAST(d[14]),
							CAST(d[15]), CAST(d[16]),
							CAST(d[17]), CAST(d[18]),
							(bool)(CAST(d[19])), (bool)(CAST(d[20])));
					}
					break;
				case SCR_SELCLEAR:
					Selector::Clear();
					rv = true;
					break;
				case SCR_SELCONFIRM:
					if(rv = Copy(&i, 1))
					{
						int _tdi = CAST(d[0]);
						CINT(d[_tdi].value) = Selector::confirm(mp.tex[TEX_WORD]);
						d[_tdi].bfloat = false;
					}
					break;
				case SCR_SELSET:
					if(rv = Copy(&i, 3))
					{
						Selector::nselect = CAST(d[0]);
						Selector::select = CAST(d[1]);
						Selector::updown = (bool)(CAST(d[2]));
					}
					break;
				case SCR_ISELBUILD:
					if(rv = Copy(&i, 7))
					{
						InfoSelect::Build(CAST(d[0]), strdesc[CINT(d[1].value)], CAST(d[2]), CAST(d[3]), CAST(d[4]), (bool)(CAST(d[5])), (bool)(CAST(d[6])));
					}
					break;
				case SCR_ISELCLEAR:
					Selector::Clear();
					rv = true;
					break;
				case SCR_ISELSET:
					if(rv = Copy(&i, 3))
					{
						InfoSelect::nselect = CAST(d[0]);
						InfoSelect::select = CAST(d[1]);
						InfoSelect::updown = (bool)(CAST(d[2]));
					}
					break;
				}
				break;
			}
			break;
			/************************************************************************/
			/* Control                                                              */
			/************************************************************************/
		case SCRKW_CONTROL:
			switch (nowval)
			{
			case SCR_IF:
nest:
				if(rv = Copy(&i, 1))
				{
					if(!CAST(d[0]))
					{
						while(true)
						{
							++i;
							if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_THEN)
								depth++;
							if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_END)
							{
								depth--;
								if(!depth)
									break;
							}
						}
						++i;
						if(i!=pnow->end())
						{
							if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_ELSE)
								continue;
							else if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_ELSEIF)
								goto nest;
							else
								--i;
						}
						else
							--i;
					}
				}
				break;
			case SCR_ELSE:
			case SCR_ELSEIF:
				if(true)
				{
					while(true)
					{
						++i;
						if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_THEN)
							depth++;
						if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_END)
						{
							depth--;
							if(!depth)
								break;
						}
					}
				}
				rv = true;
				break;
			case SCR_END:
			case SCR_THEN:
				continue;

			case SCR_LOOP:
				if(rv = Copy(&i, 1))
				{
					vector<Script> _while;
					while(true)
					{
						++i;
						if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_THEN)
							depth++;
						else if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_END)
						{
							depth--;
							if(!depth)
								break;
						}
						_while.push_back(*i);
					}
					vector<Script> * psaved = pnow;
					pnow = &_while;

					while(CINT(d[CINT(d[0].value)].value)--)
					{
						int _idesc;
						_idesc = idesc[0];
						if(!Parse())
							break;
						idesc[0] = _idesc;
					}
					pnow = psaved;
				}
				break;

			case SCR_CHATON:
				if(rv = Copy(&i, 1))
				{
					if(Player::p.ID>>1 == CAST(d[0]))
					{
						if(!chat.chati)
						{
							if(rv = Copy(&i, 3))
							{
								if(chat.chatOn(CAST(d[0]), CAST(d[1]), (bool)(CAST(d[2]))))
									chat.chati++;
								goto chatout;
							}
						}
						else
						{
							if(chat.chati == 0xff)
							{
								chat.chati = 0;
								goto chatout;
							}
							for(int j=0;j<chat.chati;j++)
							{
								++i;
								++i;
								++i;
							}
							++i;
							if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_CHATOFF)
							{
								if(chat.chatOff())
									chat.chati = 0xff;
								break;
							}
							else
							{
								--i;
								if(rv = Copy(&i, 3))
								{
									if(chat.chat(CAST(d[0]), (bool)(CAST(d[1])), strdesc[CINT(d[2].value)]))
										chat.chati++;
									goto chatout;
								}
							}
						}
					}
					else
					{
chatout:
						while(true)
						{
							++i;
							if(i->type & SCR_TOKEN_COMMAND && i->value == SCR_CHATOFF)
							{
								rv = true;
								break;
							}
						}
					}
				}
				break;
			case SCR_CHATOFF:
				continue;
			}
			break;
			/************************************************************************/
			/* Data                                                                 */
			/************************************************************************/
		case SCRKW_DATA:
			switch (nowval)
			{
			case SCR_DATAGET:
				if (rv = Copy(&i, 4))
				{
					int _tdi = CAST(d[3]);
					CINT(d[_tdi].value) = data.iRead(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
					d[_tdi].bfloat = false;
				}
				break;
			case SCR_DATAGETf:
				if (rv = Copy(&i, 4))
				{
					int _tdi = CAST(d[4]);
					CFLOAT(d[_tdi].value) = data.fRead(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
					d[_tdi].bfloat = true;
				}
				break;
			case SCR_DATASET:
				if (rv = Copy(&i, 3))
				{
					data.iWrite(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
				}
				break;
			case SCR_DATASETf:
				if (rv = Copy(&i, 3))
				{
					data.fWrite(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
				}
				break;

			case SCR_TRYSTAGE:
				if (rv = true)
				{
					DataConnector::Try();
				}
				break;

			case SCR_DEBUG_BREAKPOINT:
#ifdef __DEBUG
				if(rv = true)
				{
					LogOut();
				}
#endif
				break;
			}
			break;
		}

		if(!rv)
		{
#ifdef __DEBUG
			HGELOG("%s\nError in parsing %d.", HGELOG_ERRSTR, i->value);
			LogOut();
#endif
			return false;
		}
	}
	return true;
}

void Scripter::LogOut()
{
#ifdef __DEBUG_LOGOUT
	HGELOG("\n*****DEBUG LOGOUT START*****\n");
	HGELOG("*****DEBUG*****\nFuction Descs:");
	for(int i=0; i<SCR_VARBEGIN; i++)
	{
		if(d[i].bfloat)
			HGELOG("Desc[%d] = %f", i, CFLOAT(d[i].value));
		else
			HGELOG("Desc[%d] = %d", i, CINT(d[i].value));
	}
	HGELOG("*****DEBUG*****\nVariable Descs:");
	for(int i=SCR_VARBEGIN; i<SCR_FREEBEGIN ;i++)
	{
		if(d[i].bfloat)
			HGELOG("Var[%d] = %f", i-SCR_VARBEGIN, CFLOAT(d[i].value));
		else
			HGELOG("Var[%d] = %d", i-SCR_VARBEGIN, CINT(d[i].value));
	}
	HGELOG("*****DEBUG*****\nFree Descs:");
	for(int i=SCR_FREEBEGIN; i<SCR_RESERVEBEGIN; i++)
	{
		if(d[i].bfloat)
			HGELOG("_%d = %f", i-SCR_FREEBEGIN, CFLOAT(d[i].value));
		else
			HGELOG("_%d = %d", i-SCR_FREEBEGIN, CINT(d[i].value));
	}
	HGELOG("\n*****DEBUG LOGOUT OVER*****\n");
#endif
}

bool Scripter::Execute(vector<File> * ptype, DWORD name, DWORD con)
{
	bool bFound = false;
	bool rv = false;
	nowName = name;
	nowCon = con;
	for(vector<File>::iterator i=ptype->begin();i!=ptype->end();i++)
	{
		if(i->name == name)
		{
			for(vector<Block>::iterator j=i->file.begin();j!=i->file.end();j++)
			{
				if(j->con > SCR_TIMECONTROL)
				{
					bFound = true;
					DWORD _timefunc0 = j->con & ~SCR_TIMECONTROL;
					DWORD _timefunc = _timefunc0 & ~SCR_TIMECONTROL_TEST0;
					DWORD _con = con;
					while (_timefunc)
					{
						++j;
						if (bFound)
						{
							switch (_timefunc & SCRKT_TEST_OPERATOR)
							{
							case SCR_TIMEFUNC_MOD:
								_con = _con % j->con;
								break;
							case SCR_TIMEFUNC_PLUS:
								_con += j->con;
								break;
							case SCR_TIMEFUNC_LESS:
								if (_con >= j->con)
								{
									bFound = false;
								}
								break;
							case SCR_TIMEFUNC_LESSEQUAL:
								if (_con > j->con)
								{
									bFound = false;
								}
								break;
							case SCR_TIMEFUNC_EQUAL:
								if (_con != j->con)
								{
									bFound = false;
								}
							case SCR_TIMEFUNC_GREATEQUAL:
								if (_con < j->con)
								{
									bFound = false;
								}
								break;
							case SCR_TIMEFUNC_GREAT:
								if (_con <= j->con)
								{
									bFound = false;
								}
								break;
							default:
								bFound = false;
								break;
							}
						}
						_timefunc = _timefunc >> SCRKT_SIZE_OPERATOR;
					}

					if ((_timefunc0 & SCR_TIMECONTROL_TEST0) && _con != 0)
					{
						bFound = false;
					}

					if (bFound)
					{
						pnow = &(j->block);
					}
				}
				else if(j->con == con)
				{
					pnow = &(j->block);
					bFound = true;
				}
				if(bFound)
				{
					if(!(rv = Parse()))
					{
#ifdef __DEBUG
						HGELOG("%s\nError in parsing script! At Block %d of File %d.", HGELOG_ERRSTR, j->con, i->name);
#endif
						break;
					}
					bFound = false;
				}
			}
			break;
		}
	}

	return rv;
}

bool Scripter::Copy(vector<Script>::iterator * p, BYTE num)
{
	if(num > SCR_MAXDESC)
		return false;
	for(int i=0; ; i++)
	{
		vector<Script>::iterator ts = ++(*p);
		if(i == num && ((*p) == pnow->end() || !((*p)->type & SCR_TOKEN_OPERATOR)))
		{
			--(*p);
			break;
		}
		if(ts->type & SCR_TOKEN_OPERATOR)
		{
			switch(ts->value)
			{
			case SCR_ADD_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi + CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_SUB_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi - CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_MUL_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi * CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_DIV_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi / CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_ADD_FLOAT:
				i--;
				tdf = CAST(d[i]);
				fdesc[i] = tdf + CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &fdesc[i];
				d[i].bfloat = true;
				break;
			case SCR_SUB_FLOAT:
				i--;
				tdf = CAST(d[i]);
				fdesc[i] = tdf - CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &fdesc[i];
				d[i].bfloat = true;
				break;
			case SCR_MUL_FLOAT:
				i--;
				tdf = CAST(d[i]);
				fdesc[i] = tdf * CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &fdesc[i];
				d[i].bfloat = true;
				break;
			case SCR_DIV_FLOAT:
				i--;
				tdf = CAST(d[i]);
				fdesc[i] = tdf / CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &fdesc[i];
				d[i].bfloat = true;
				break;
			case SCR_ADD_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu + CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_SUB_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu - CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_MUL_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu * CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_DIV_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu / CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_MOD:
				i--;
				tdi = CINT(d[i].value);
				idesc[i] = tdi % CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_MOD_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu % CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;

			case SCR_INVERSE:
				i--;
				idesc[i] = ~ idesc[i];
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_NOT:
				i--;
				idesc[i] = ! idesc[i];
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;

			case SCR_GREAT_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi > CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_LESS_INT:
				i--;
				tdi = CAST(d[i]);
				idesc[i] = tdi < CINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_GREAT_FLOAT:
				i--;
				tdf = CAST(d[i]);
				idesc[i] = tdf > CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_LESS_FLOAT:
				i--;
				tdf = CAST(d[i]);
				idesc[i] = tdf < CFLOAT(ValueF(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_GREAT_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu > CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			case SCR_LESS_UINT:
				i--;
				tdu = UCAST(d[i]);
				idesc[i] = tdu < CUINT(ValueI(&(++(*p)), i));
				d[i].value = &idesc[i];
				d[i].bfloat = false;
				break;
			}
		}
		else
		{
			d[i].value = Value(&(*p), i, 0);
		}
		if(*p == pnow->end())
			return false;
	}
	return true;
}

void * Scripter::Value(vector<Script>::iterator * p, int i, BYTE force)
{
	if((*p)->type & SCR_TOKEN_VALUE)
	{
		if(force & SCR_FORCE_INT)
		{
			d[i].bfloat = false;
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				idesc[i] = (int)CFLOAT(&((*p)->value));
			}
			else
			{
				idesc[i] = (int)CINT(&((*p)->value));
			}
		}
		else if(force & SCR_FORCE_FLOAT)
		{
			d[i].bfloat = true;
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				fdesc[i] = (float)CFLOAT(&((*p)->value));
			}
			else
			{
				fdesc[i] = (float)CINT(&((*p)->value));
			}
		}
		else
		{
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				d[i].bfloat = true;
				fdesc[i] = CFLOAT(&((*p)->value));
			}
			else
			{
				d[i].bfloat = false;
				idesc[i] = CINT(&((*p)->value));
			}
		}
	}
	else if((*p)->type & SCR_TOKEN_HANDLE)
	{
		d[i].bfloat = false;
		idesc[i] = (*p)->value;
	}
	else if((*p)->type & SCR_TOKEN_VARIABLE)
	{
		int _tdi = (*p)->value;
		d[i].bfloat = d[_tdi].bfloat;
		if(d[i].bfloat)
			fdesc[i] = CFLOAT(d[_tdi].value);
		else
			idesc[i] = CINT(d[_tdi].value);
	}
	else
	{
		int _tdi;
		float _tdf;

		DWORD nowval = (*p)->value;
		switch(nowval & SCRKWMASK_CLASS)
		{
			/************************************************************************/
			/* AttackObject                                                         */
			/************************************************************************/
		case SCRKW_CLASS_ATTACKOBJECT:
			switch (nowval)
			{
			case SCR_BUI:
				idesc[i] = Bullet::index;
				d[i].bfloat = false;
				break;
			case SCR_BUANGLE:
				idesc[i] = bu[Bullet::index].angle;
				d[i].bfloat = false;
				break;
			case SCR_BUSPEED:
				fdesc[i] = bu[Bullet::index].speed;
				d[i].bfloat = true;
				break;
			case SCR_BUX:
				fdesc[i] = bu[Bullet::index].x;
				d[i].bfloat = true;
				break;
			case SCR_BUY:
				fdesc[i] = bu[Bullet::index].y;
				d[i].bfloat = true;
				break;
			case SCR_BUTIMER:
				idesc[i] = bu[Bullet::index].timer;
				d[i].bfloat = false;
				break;

			case SCR_BEI:
				idesc[i] = Beam::index;
				d[i].bfloat = false;
				break;
			case SCR_BEANGLE:
				idesc[i] = be[Beam::index].angle;
				d[i].bfloat = false;
				break;
			case SCR_BESPEED:
				fdesc[i] = be[Beam::index].speed;
				d[i].bfloat = true;
				break;
			case SCR_BEX:
				fdesc[i] = be[Beam::index].x;
				d[i].bfloat = true;
				break;
			case SCR_BEY:
				fdesc[i] = be[Beam::index].y;
				d[i].bfloat = true;
				break;
			case SCR_BETIMER:
				idesc[i] = be[Beam::index].timer;
				d[i].bfloat = false;
				break;
			case SCR_BEHOLDTAR:
				idesc[i] = be[Beam::index].holdtar;
				d[i].bfloat = false;
				break;
			case SCR_BEPINTAR:
				idesc[i] = be[Beam::index].pintar;
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Ghost                                                                */
			/************************************************************************/
		case SCRKW_CLASS_GHOST:
			switch (nowval)
			{
			case SCR_GHX:
				fdesc[i] = gh[Ghost::index].x;
				d[i].bfloat = true;
				break;
			case SCR_GHY:
				fdesc[i] = gh[Ghost::index].y;
				d[i].bfloat = true;
				break;
			case SCR_GHTIMER:
				idesc[i] = gh[Ghost::index].timer;
				d[i].bfloat = false;
				break;
			case SCR_GHI:
				idesc[i] = Ghost::index;
				d[i].bfloat = false;
				break;
			case SCR_GHANGLE:
				idesc[i] = gh[Ghost::index].angle;
				d[i].bfloat = false;
				break;
			case SCR_GHSPEED:
				fdesc[i] = gh[Ghost::index].speed;
				d[i].bfloat = true;
				break;
			case SCR_GHAMAP:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = gh[Ghost::index].aMainAngle(Player::p, _tdi);
				d[i].bfloat = false;
				break;
			case SCR_GHRMAP:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = gh[Ghost::index].rMainAngle(Player::p, _tdi);
				d[i].bfloat = false;
				break;
			case SCR_GHAIMX:
				fdesc[i] = gh[Ghost::index].aim.x;
				d[i].bfloat = true;
				break;
			case SCR_GHAIMY:
				fdesc[i] = gh[Ghost::index].aim.y;
				d[i].bfloat = true;
				break;
			case SCR_GHAC:
				idesc[i] = gh[Ghost::index].ac;
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Enemy                                                                */
			/************************************************************************/
		case SCRKW_CLASS_ENEMY:
			switch (nowval)
			{
			case SCR_ENX:
				fdesc[i] = en[Enemy::index].x;
				d[i].bfloat = true;
				break;
			case SCR_ENY:
				fdesc[i] = en[Enemy::index].y;
				d[i].bfloat = true;
				break;
			case SCR_ENTIMER:
				idesc[i] = en[Enemy::index].timer;
				d[i].bfloat = false;
				break;
			case SCR_ENI:
				idesc[i] = Enemy::index;
				d[i].bfloat = false;
				break;
			case SCR_ENANGLE:
				idesc[i] = en[Enemy::index].angle;
				d[i].bfloat = false;
				break;
			case SCR_ENSPEED:
				fdesc[i] = en[Enemy::index].speed;
				d[i].bfloat = true;
				break;
			case SCR_ENAMAP:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = en[Enemy::index].aMainAngle(Player::p, _tdi);
				d[i].bfloat = false;
				break;
			case SCR_ENRMAP:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = en[Enemy::index].rMainAngle(Player::p, _tdi);
				d[i].bfloat = false;
				break;
			case SCR_ENAIMX:
				fdesc[i] = en[Enemy::index].aim.x;
				d[i].bfloat = true;
				break;
			case SCR_ENAIMY:
				fdesc[i] = en[Enemy::index].aim.y;
				d[i].bfloat = true;
				break;
			}
			break;
			/************************************************************************/
			/* Math                                                                 */
			/************************************************************************/
		case SCRKW_CLASS_MATH:
			switch (nowval)
			{
			case SCR_RAND:
				idesc[i] = rand();
				d[i].bfloat = false;
				break;
			case SCR_RANDA:
				idesc[i] = RANDA;
				d[i].bfloat = false;
				break;
			case SCR_RANDR:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = rand() % (_tdi * 2 + 1) - _tdi;
				d[i].bfloat = false;
				break;
			case SCR_SEED:
				idesc[i] = mp.seed;
				d[i].bfloat = false;
				break;
			case SCR_SINA:
				++(*p);
				fdesc[i] = sint(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;
				break;
			case SCR_COSA:
				++(*p);
				fdesc[i] = cost(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;
				break;
			case SCR_TANA:
				++(*p);
				fdesc[i] = tant(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;
				break;
			case SCR_ATAN2:
				++(*p);
				_tdf = CFLOAT((Value(&(*p), i, 0)));
				++(*p);
				idesc[i] = ANGLE(atan2f(_tdf, CFLOAT((Value(&(*p), i, 0)))));
				d[i].bfloat = false;
				break;
			case SCR_SQRT:
				++(*p);
				fdesc[i] = sqrtf(CFLOAT((Value(&(*p), i, 0))));
				d[i].bfloat = true;
				break;
			case SCR_SIGN:
				++(*p);
				idesc[i] = SIGN(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;
				break;
			case SCR_ROLL:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				++(*p);
				idesc[i] = ROLL(_tdi, CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Other                                                                */
			/************************************************************************/
		case SCRKW_CLASS_OTHER:
			switch (nowval)
			{
			case SCR_D:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				if(d[_tdi].bfloat)
					fdesc[i] = CAST(d[_tdi]);
				else
					idesc[i] = CAST(d[_tdi]);
				d[i].bfloat = d[_tdi].bfloat;
				break;
			case SCR_DU:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = UCAST(d[_tdi]);
				d[i].bfloat = false;
				break;
			case SCR_TX:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = tar[_tdi].x;
				d[i].bfloat = true;
				break;
			case SCR_TY:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = tar[_tdi].y;
				d[i].bfloat = true;
				break;
			case SCR_TIME:
				idesc[i] = time;
				d[i].bfloat = false;
				break;
			case SCR_NOWNAME:
				idesc[i] = nowName;
				d[i].bfloat = false;
				break;
			case SCR_NOWCON:
				idesc[i] = nowCon;
				d[i].bfloat = false;
				break;

			case SCR_DIFFI:
				idesc[i] = mp.nowdifflv;
				d[i].bfloat = false;
				break;
			case SCR_CHARA:
				idesc[i] = mp.luchara;
				d[i].bfloat = false;
				break;
			case SCR_GETSCENE:
				idesc[i] = mp.scene;
				d[i].bfloat = false;
				break;
			case SCR_MODE:
				idesc[i] = (mp.practicemode ? 2:0) | (mp.rangemode ? 1:0);
				d[i].bfloat = false;
				break;
			case SCR_REPLAYMODE:
				idesc[i] = mp.replaymode;
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Special                                                              */
			/************************************************************************/
		case SCRKW_CLASS_SPECIAL:
			switch (nowval)
			{
			case SCR_CHATI:
				idesc[i] = chat.chati;
				d[i].bfloat = false;
				break;

			case SCR_BOSSFAILED:
				idesc[i] = BossInfo::failed;
				d[i].bfloat = false;
				break;
			case SCR_BOSSFLAG:
				idesc[i] = BossInfo::flag;
				d[i].bfloat = false;
				break;

			case SCR_CHECKKEY:
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				++(*p);
				idesc[i] = hge->Input_GetDIKey(_tdi, CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Effect                                                               */
			/************************************************************************/
		case SCRKW_CLASS_EFFECT:
			switch (nowval)
			{
			}
			break;
			/************************************************************************/
			/* Player                                                               */
			/************************************************************************/
		case SCRKW_CLASS_PLAYER:
			switch (nowval)
			{
			case SCR_PX:
				fdesc[i] = Player::p.x;
				d[i].bfloat = true;
				break;
			case SCR_PY:
				fdesc[i] = Player::p.y;
				d[i].bfloat = true;
				break;
			case SCR_PLIFE:
				idesc[i] = Player::p.nLife;
				d[i].bfloat = false;
				break;
			case SCR_PBOMB:
				idesc[i] = Player::p.nBomb;
				d[i].bfloat = false;
				break;
			case SCR_PPOWER:
				idesc[i] = Player::p.nPower;
				d[i].bfloat = false;
				break;
			case SCR_PALIVENESS:
				idesc[i] = Player::p.nAliveness;
				d[i].bfloat =false;
				break;
			case SCR_PPOINT:
				idesc[i] = Player::p.nPoint;
				d[i].bfloat = false;
				break;
			case SCR_PBCIRCLE:
				idesc[i] = (int)(Player::p.bCircle);
				d[i].bfloat = false;
				break;
			case SCR_PBSLOW:
				idesc[i] = (int)(Player::p.bSlow);
				d[i].bfloat = false;
				break;
			case SCR_PBINFI:
				idesc[i] = (int)(Player::p.bInfi);
				d[i].bfloat = false;
				break;
			case SCR_PSPEED:
				fdesc[i] = Player::p.speed;
				d[i].bfloat = true;
				break;
			case SCR_PSLOWSPEED:
				fdesc[i] = Player::p.slowspeed;
				d[i].bfloat = true;
				break;
			}
			break;
			/************************************************************************/
			/* Layer                                                                */
			/************************************************************************/
		case SCRKW_CLASS_LAYER:
			switch (nowval)
			{
			case SCR_BGSTIMER:
				idesc[i] = BGLayer::set[BGLayer::setindex].timer;
				d[i].bfloat = false;
				break;
			}
			break;
			/************************************************************************/
			/* Select                                                               */
			/************************************************************************/
		case SCRKW_CLASS_SELECT:
			switch (nowval)
			{
			case SCR_SELCOMPLETE:
				idesc[i] = Selector::complete;
				d[i].bfloat = false;
				break;
			case SCR_SEL:
				idesc[i] = Selector::select;
				d[i].bfloat = false;
				break;
			case SCR_ISELCOMPLETE:
				idesc[i] = InfoSelect::complete;
				d[i].bfloat = false;
				break;
			case SCR_ISEL:
				idesc[i] = InfoSelect::select;
				d[i].bfloat = false;
				break;
			}
			break;
		}
	}

	if(d[i].bfloat)
		return &fdesc[i];
	return &idesc[i];
}