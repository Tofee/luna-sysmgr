/* @@@LICENSE
*
*      Copyright (c) 2008-2013 LG Electronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* LICENSE@@@ */




#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "Common.h"

#include "Timer.h"
#include "Mutex.h"
#include "Event.h"
#include "sptr.h"
#include "AmbientLightSensor.h"
#include "DisplayStates.h"
#include "SuspendBlocker.h"

#include <QEvent>
#include <QObject>

#include "lunaservice.h"

#define DISPLAY_LOCK_SCREEN   1
#define DISPLAY_UNLOCK_SCREEN 2
#define DISPLAY_NIGHT_SCREEN  3
#define DISPLAY_DOCK_SCREEN     4

#define DISPLAY_EXIT_NIGHT_STAND_MODE   1001

#define DISPLAY_SIGNAL_OFF 2001
#define DISPLAY_SIGNAL_ON  2002
#define DISPLAY_SIGNAL_DIM 2003
#define DISPLAY_SIGNAL_DOCK 2004
#define DISPLAY_SIGNAL_OFF_ON_CALL 2005

#define DISPLAY_ALERT_GENERIC_ACTIVATED     1
#define DISPLAY_ALERT_PHONECALL_ACTIVATED   2
#define DISPLAY_ALERT_GENERIC_DEACTIVATED   3
#define DISPLAY_ALERT_PHONECALL_DEACTIVATED 4

#define DISPLAY_BANNER_ACTIVATED            5
#define DISPLAY_BANNER_DEACTIVATED          6

#define DISPLAY_ALERT_CANCEL                7

#define ACTIVE_MESSAGE_PATTERN_SIZE         6

#define DISPLAY_EVENT_REQUEST     0
#define DISPLAY_EVENT_ON          1
#define DISPLAY_EVENT_DIMMED      2
#define DISPLAY_EVENT_OFF         3
#define DISPLAY_EVENT_TIMEOUTS    4
#define DISPLAY_EVENT_PUSH_DNAST  5
#define DISPLAY_EVENT_POP_DNAST   6
#define DISPLAY_EVENT_ACTIVE      7
#define DISPLAY_EVENT_INACTIVE    8
#define DISPLAY_EVENT_DOCKMODE    9

#define CHARGER_NONE              0
#define CHARGER_USB               1
#define CHARGER_INDUCTIVE         (1 << 1)

#define DISPLAY_EVENT_NONE                           100
#define DISPLAY_EVENT_SLIDER_LOCKED                  101
#define DISPLAY_EVENT_SLIDER_UNLOCKED                102
#define DISPLAY_EVENT_POWER_BUTTON_UP                103
#define DISPLAY_EVENT_POWER_BUTTON_DOWN              104
#define DISPLAY_EVENT_INDUCTIVE_CHARGER_DISCONNECTED 105
#define DISPLAY_EVENT_INDUCTIVE_CHARGER_CONNECTED    106
#define DISPLAY_EVENT_USB_CHARGER_DISCONNECTED       107
#define DISPLAY_EVENT_USB_CHARGER_CONNECTED          108
#define DISPLAY_EVENT_ALS_REGION_CHANGED             109
#define DISPLAY_EVENT_ENTER_EMERGENCY_MODE           110
#define DISPLAY_EVENT_EXIT_EMERGENCY_MODE            111
#define DISPLAY_EVENT_PROXIMITY_ON                   112
#define DISPLAY_EVENT_PROXIMITY_OFF                  113
#define DISPLAY_EVENT_ON_CALL                        114
#define DISPLAY_EVENT_OFF_CALL                       115
#define DISPLAY_EVENT_HOME_BUTTON_UP                 116

class DisplayManager : public QObject
{
	Q_OBJECT

public:

    DisplayManager();
    static DisplayManager* instance ();

    virtual ~DisplayManager();
    GMainLoop* mainLoop();

    void handlePowerKey(bool pressed);

    void handleDisplayEvent(DisplayEvent event);
    void handleTouchEvent();

    void wakeupDevice(const char *reason);

    bool alert (int state);
    uint32_t getCoreNaviBrightness();

    // service notifications for services used
    static bool powerdServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
    static bool telephonyServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
    static bool audiodServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
    static bool keysServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
    static bool hiddServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
    static bool bootMgrServiceNotification(LSHandle *sh, const char *serviceName, bool connected, void *ctx);

    // luna-service methods
    static bool controlSetState(LSHandle *sh, LSMessage *message, void *ctx);
    static bool cancelSubscription(LSHandle *sh, LSMessage *message, void *ctx);
    static bool controlStatus(LSHandle *sh, LSMessage *message, void *ctx);
    static bool controlGetProperty(LSHandle *sh, LSMessage *message, void *ctx);
    static bool controlSetProperty(LSHandle *sh, LSMessage *message, void *ctx);
    static bool controlCallStatus(LSHandle *sh, LSMessage *message, void *ctx);
	static bool controlLockStatus(LSHandle *sh, LSMessage *message, void *ctx);
	static bool controlSetLockStatus(LSHandle *sh, LSMessage *message, void *ctx);
    static bool controlAlert(LSHandle *sh, LSMessage *message, void *ctx);

    // service callbacks
    static bool timeoutCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool connectedOnCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool batteryCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool chargerCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool usbDockCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool sliderCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool audiodCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static void powerdCallback (void);
    static bool bootStatusCallback(LSHandle *sh, LSMessage *message, void *ctx);

    static void getBearingInfo(double& latitude, double& longitude);
    static void setBearingInfo(double latitude, double longitude);

    bool pushDNAST (const char *id);
    bool popDNAST (const char *id);
    bool updateState (int eventType);
    bool cancelLockTimer();
    void setActiveTouchpanel (bool enable);
    void setAlsDisabled (bool disable);

    bool orientationSensorOn();
    bool orientationSensorOff();

    bool updateNyxWithLocation(double latitude, double longitude);

    bool isOn() const;
    bool isUSBCharging() const;
    bool isOnCall() const;
    bool isSliderOpen() const;
    bool isDNAST() const;
    bool isOnPuck() const;
    bool hasSlider() const;
    bool isProximityActivated() const;
    bool isLocked() const;
    bool isBacklightOn() const;
    bool isDisplayOn() const;
    bool isTouchpanelOn() const;
    bool isDemo() const;
    bool isBootFinished() const;

    std::string puckId() const;

    int dimTimeout() const;
    int offTimeout() const;
    int lockedOffTimeout() const;
    int lastEvent() const;
    int lastUserEvent() const;
    int lastInternalEvent() const;

    bool setMaximumBrightness (int maxBrightness, bool save=true);
    int  getMaximumBrightness () { return m_maxBrightness; }

    void unlock();
    void lock();

    bool dock();
    bool undock();

    bool allowSuspend();
    void setSuspended (bool suspended);

	static void forceVsyncOff(bool forceNoVsync);
	static bool isVsyncOff();

private:
    static DisplayManager* m_instance;
    static bool            s_forceVsyncDisable;
    static bool            s_vsyncEnabled;
    static double		   s_currentLatitude;
    static double		   s_currentLongitude;

    LSHandle*              m_service;

    AmbientLightSensor*    m_als;
    bool		   m_alsDisabled;
    bool                   m_powerdOnline;
    int32_t                m_chargerConnected;
    int32_t                m_batteryL;

    bool                   m_onWhenConnected;
    bool                   m_drop_key;
    bool                   m_drop_pen;
    bool                   m_allow_move;
    int32_t                m_dnast;
    int32_t                m_blockedPowerKey;

    bool                   m_hasSlider;
    bool                   m_sliderOpen;
    enum                   {SLIDING_NOW = 2, SLIDING_WAIT = 1, NOT_SLIDING = 0} m_slidingNow;
    uint32_t               m_slidingStart;

    bool                   m_dropPowerKey;
    bool                   m_penDown;
    uint32_t               m_blockId;
    uint32_t               m_metaId;
    uint32_t               m_lastEvent;
    uint32_t               m_lastUserEvent;
    uint32_t               m_lastInternalEvent;
    uint32_t               m_lastKey;
    int32_t                m_dimTimeout;
    int32_t                m_offTimeout;
    int32_t                m_totalTimeout;
    int32_t                m_lockedOffTimeout;
    int32_t                m_activityTimeout;
    int32_t                m_powerKeyTimeout;
    bool                   m_onCall;
    bool		   m_demo;
    bool		   m_homeKeyDown;

    bool		   m_bootFinished;

    int32_t                m_alertState;
    int32_t                m_proximityCount;
    bool                   m_proximityEnabled;
    bool                   m_proximityActivated;

    LSMessageToken         m_calbackOnToken;
    LSMessageToken         m_calbackOffToken;

    bool                   m_displayOn;
    bool                   m_touchpanelIsOn;
    bool                   m_backlightIsOn;
    bool		   m_activeTouchpanel;
    Timer<DisplayManager>* m_activity;
    Timer<DisplayManager>* m_power;
    Timer<DisplayManager>* m_slider;
    Timer<DisplayManager>* m_alertTimer;
    int32_t                m_maxBrightness;

    std::string 	   m_puckId;

    DisplayStateBase* m_currentState;
    DisplayStateBase** m_displayStates;
    DisplayLockState  m_lockState;

    SuspendBlocker<DisplayManager> m_suspendBlocker;

    bool m_powerKeyPressEventScheduled;

    bool off (sptr<Event> event = 0);
    bool on (sptr<Event> event = 0);
    bool dim (sptr<Event> event = 0);

    bool timeout();
    bool activity();
    bool power();
    bool slider();
    bool alertTimerCallback();
    bool updateTimeout(int timeoutInMs);
    bool setTimeout (int timeout);
    bool notifySubscribers(int type, sptr<Event> event = 0);
    bool updateBrightness ();
    int32_t getDisplayBrightness ();
    int32_t getKeypadBrightness ();
    DisplayState currentState() const;
    void setTouchpanelMode (bool active);

    friend class DisplayStateBase;

    void	changeDisplayState (DisplayState newDisplayState, DisplayState oldDisplayState, DisplayEvent displayEvent, sptr<Event> event);
    bool	updateLockState (DisplayLockState lockState, DisplayState state, DisplayEvent displayEvent);
    // used by DisplayStateBase class to change current state. Returns false is target lockState couldn't be reached

    void handleLockStateChange(int state, int displayEvent);

    void displayOn(bool als);
    void displayAdjust();
    void displayOff();
    void displayDim();

    bool proximityOn();
    bool proximityOff();

    void emitDisplayStateChange (int displaySignal);
    void requestCurrentLocation();

    // internal methods
    void backlightOn (int displayBrightness, int keyBrightness);
    static void backlightOnCallback (void* ctx);
    void backlightOff();
    static void backlightOffCallback (void* ctx);

    bool touchPanelOn();
    bool touchPanelOff();
    static bool touchPanelOffCallback (LSHandle* sh, LSMessage* message, void* ctx);
    static bool updateCompassBearingInfo(LSHandle* sh, LSMessage* message, void* ctx);
    static bool displayBlankedCallback(LSHandle *service, LSMessage *message, void *user_data);

    void initStates();
    void clearStates();

	static void changeVsyncControl(bool enable);

	void markBootFinished(bool finished);

    void updateCompositorDisplayState(bool on, LSMethodFunction cb , void *context);

    static bool displayOnCallback(LSHandle *handle, LSMessage *message, gpointer context);
    static bool displayOffCallback(LSHandle *handle, LSMessage *message, gpointer context);

	bool unlockRequiresPasscode() const;

private Q_SLOTS:

    void slotEmergencyMode (bool enable);
    void slotAlsEnabled (bool enable);
    void slotShowIME();
    void slotHideIME();
    void slotBluetoothKeyboardActive(bool active);
    void slotAirplaneModeChanged(bool change);

Q_SIGNALS:

    void signalDisplayStateChange(int state);
	void signalPuckConnected(bool connected);
    void signalDisplayMaxBrightnessChanged(int brightness);
};


#endif /* DISPLAYMANAGER_H */

