#ifndef __BROWSER_HANDLER_H__
#define __BROWSER_HANDLER_H__
#pragma once
#include <set>
#include <string>
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"

namespace Browser
{
	class ClientHandler
		: public CefClient				// 浏览器客户区
		, public CefContextMenuHandler	// 浏览器上下文菜单处理
		, public CefDisplayHandler		// 浏览器显示处理
		, public CefDownloadHandler		// 浏览器下载处理
		, public CefDragHandler
		, public CefGeolocationHandler	// 浏览器地理定位处理
		, public CefJSDialogHandler		// 浏览器JS对话框处理
		, public CefKeyboardHandler		// 浏览器键盘处理
		, public CefLifeSpanHandler		// 浏览器生命周期处理
		, public CefLoadHandler			// 浏览器加载处理
		, public CefRequestHandler		// 浏览器请求处理
	{
	public:
		class Delegate {
		public:
			virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;
			virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;
			virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) = 0;
			virtual void OnSetAddress(const std::wstring& url) = 0;
			virtual void OnSetTitle(const std::wstring& title) = 0;
			virtual void OnSetFullscreen(bool fullscreen) = 0;
			virtual void OnSetLoadingState(bool isLoading,bool canGoBack,bool canGoForward) = 0;
			virtual void OnSetDraggableRegions(const std::vector<CefDraggableRegion>& regions) = 0;

		protected:
			virtual ~Delegate() {}
		};
		typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

	public:
		ClientHandler(Delegate* delegate,bool is_osr,const std::wstring& startup_url);
		void DetachDelegate();

		//////////////////////////////////////////////////////////////////////////
		// CefClient methods
		CefRefPtr<CefContextMenuHandler>	GetContextMenuHandler()	OVERRIDE {return this;}
		CefRefPtr<CefDisplayHandler>		GetDisplayHandler()		OVERRIDE {return this;}
		CefRefPtr<CefDownloadHandler>		GetDownloadHandler()	OVERRIDE {return this;}
		CefRefPtr<CefDragHandler>			GetDragHandler()		OVERRIDE {return this;}
		CefRefPtr<CefGeolocationHandler>	GetGeolocationHandler()	OVERRIDE {return this;}
		CefRefPtr<CefJSDialogHandler>		GetJSDialogHandler()	OVERRIDE {return this;}
		CefRefPtr<CefKeyboardHandler>		GetKeyboardHandler()	OVERRIDE {return this;}
		CefRefPtr<CefLifeSpanHandler>		GetLifeSpanHandler()	OVERRIDE {return this;}
		CefRefPtr<CefLoadHandler>			GetLoadHandler()		OVERRIDE {return this;}
		CefRefPtr<CefRequestHandler>		GetRequestHandler()		OVERRIDE {return this;}

		bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefContextMenuHandler methods
		void OnBeforeContextMenu(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model) OVERRIDE;

		bool OnContextMenuCommand(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			int command_id,
			EventFlags event_flags) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefDisplayHandler methods
		void OnAddressChange(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& url) OVERRIDE;
		void OnTitleChange(
			CefRefPtr<CefBrowser> browser,
			const CefString& title) OVERRIDE;
		void OnFaviconURLChange(
			CefRefPtr<CefBrowser> browser,
			const std::vector<CefString>& icon_urls) OVERRIDE;
		void OnFullscreenModeChange(
			CefRefPtr<CefBrowser> browser,
			bool fullscreen) OVERRIDE;
		bool OnConsoleMessage(
			CefRefPtr<CefBrowser> browser,
			const CefString& message,
			const CefString& source,
			int line) OVERRIDE;
		bool OnTooltip(
			CefRefPtr<CefBrowser> browser,
			CefString& text) OVERRIDE;
		void OnStatusMessage(
			CefRefPtr<CefBrowser> browser,
			const CefString& value) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefDownloadHandler methods
		void OnBeforeDownload(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			const CefString& suggested_name,
			CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
		void OnDownloadUpdated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefDragHandler methods
		bool OnDragEnter(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> dragData,
			CefDragHandler::DragOperationsMask mask) OVERRIDE;
		void OnDraggableRegionsChanged(
			CefRefPtr<CefBrowser> browser,
			const std::vector<CefDraggableRegion>& regions) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefGeolocationHandler methods
		bool OnRequestGeolocationPermission(
			CefRefPtr<CefBrowser> browser,
			const CefString& requesting_url,
			int request_id,
			CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefKeyboardHandler methods
		bool OnPreKeyEvent(
			CefRefPtr<CefBrowser> browser,
			const CefKeyEvent& event,
			CefEventHandle os_event,
			bool* is_keyboard_shortcut) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefLifeSpanHandler methods
		virtual bool OnBeforePopup(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			cef_window_open_disposition_t target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access) OVERRIDE;

		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefLoadHandler methods
		void OnLoadingStateChange(
			CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward) OVERRIDE;
		void OnLoadStart(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame) OVERRIDE;
		void OnLoadEnd(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) OVERRIDE;
		void OnLoadError(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// CefRequestHandler methods
		bool OnBeforeBrowse(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			bool is_redirect) OVERRIDE;
		bool OnOpenURLFromTab(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			CefRequestHandler::WindowOpenDisposition target_disposition,
			bool user_gesture) OVERRIDE;
		cef_return_value_t OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;
		//CefRefPtr<CefResourceHandler> GetResourceHandler(
		//	CefRefPtr<CefBrowser> browser,
		//	CefRefPtr<CefFrame> frame,
		//	CefRefPtr<CefRequest> request) OVERRIDE;
		//CefRefPtr<CefResponseFilter> GetResourceResponseFilter(
		//	CefRefPtr<CefBrowser> browser,
		//	CefRefPtr<CefFrame> frame,
		//	CefRefPtr<CefRequest> request,
		//	CefRefPtr<CefResponse> response) OVERRIDE;
		bool OnQuotaRequest(
			CefRefPtr<CefBrowser> browser,
			const CefString& origin_url,
			int64 new_size,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;
		void OnProtocolExecution(
			CefRefPtr<CefBrowser> browser,
			const CefString& url,
			bool& allow_os_execution) OVERRIDE;
		//bool OnCertificateError(
		//	CefRefPtr<CefBrowser> browser,
		//	ErrorCode cert_error,
		//	const CefString& request_url,
		//	CefRefPtr<CefSSLInfo> ssl_info,
		//	CefRefPtr<CefRequestCallback> callback) OVERRIDE;
		void OnRenderProcessTerminated(
			CefRefPtr<CefBrowser> browser,
			TerminationStatus status) OVERRIDE;

		// Returns the number of browsers currently using this handler. Can only be
		// called on the CEF UI thread.
		int GetBrowserCount() const;

		// Returns the Delegate.
		Delegate* delegate() const { return m_Delegate; }

		// Returns the startup URL.
		std::wstring StartupUrl() const { return m_StartupUrl; }

		// Returns true if this handler uses off-screen rendering.
		bool IsOsr() const { return m_IsOsr; }

	private:
		
		bool CreatePopupWindow(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			cef_window_open_disposition_t target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access);

		// Execute Delegate notifications on the main thread.
		void NotifyBrowserCreated(CefRefPtr<CefBrowser> browser);
		void NotifyBrowserClosing(CefRefPtr<CefBrowser> browser);
		void NotifyBrowserClosed(CefRefPtr<CefBrowser> browser);
		void NotifyAddress(const CefString& url);
		void NotifyTitle(const CefString& title);
		void NotifyFullscreen(bool fullscreen);
		void NotifyLoadingState(bool isLoading,bool canGoBack,bool canGoForward);
		void NotifyDraggableRegions(const std::vector<CefDraggableRegion>& regions);

		// True if this handler uses off-screen rendering.
		const bool m_IsOsr;

		// The startup URL.
		std::wstring m_StartupUrl;

		// Handles the browser side of query routing. The renderer side is handled
		// in client_renderer.cc.
		CefRefPtr<CefMessageRouterBrowserSide> m_MessageRouter;

		Delegate* m_Delegate;

		// The current number of browsers using this handler.
		int m_nBrowserCount;

		// Set of Handlers registered with the message router.
		MessageHandlerSet m_MessageHandlerSet;

		IMPLEMENT_REFCOUNTING(ClientHandler);
		DISALLOW_COPY_AND_ASSIGN(ClientHandler);

	public:
		CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }
		int GetBrowserId() { return m_BrowserId; }

	private:
		CefRefPtr<CefBrowser> m_Browser;
		int m_BrowserId;
	};
}

#endif
