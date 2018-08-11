///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: direct3d.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DIRECT3D_H_
#define _DIRECT3D_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <wrl\client.h>


namespace ubrot
{
namespace graphics
{
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Direct3D
/// The Direct3D class contains all components needed to use Direct3D.
///////////////////////////////////////////////////////////////////////////////////////////////////
class Direct3D
{
public:
	/**
	 * A constructor where all the member pointers are initialised to null.
	 */
	Direct3D();
	/**
	 * Empty copy contructor
	 * @param other to copy
	 */
	Direct3D(const Direct3D& other);
	~Direct3D();

	/**
	 * Needed for 16 Bit allignement
	 */
	void* operator new(size_t n) throw();
	void operator delete(void *p) throw();

	/**
	 * Method that initializes all settings needed to use DirectX 11.
	 *
	 * Uses \p screenWidth and \p screenHeight to set the window dimensions and sets
	 * \p hwnd to be the handle to that window.
	 *
	 * @param hwnd handle to the window
	 * @param screenNear distance from camera to near plane
	 * @param screenDepth distance from camera to far plane
	 * @param screenWidth width of the window
	 * @param screenHeight height of the window
	 * @param vsync lock the monitor refresh rate if \c true, else render as fast as possible
	 * @param fullscreen use windowed mode if \c false, else fullscreen
	 * @return whether everything could be initialized successfully
	 */
	bool Initialize(
		HWND hwnd,
		const float screenDepth, const float screenNear,
		const int screenWidth, const int screenHeight,
		const bool vsync, const	bool fullscreen
	);
	/**
	 * Shuts down by calling shut down for every member (where possible) and freeing allocated
	 * memory. To avoid exceptions, the \a swap \a chain is set to windowed mode.
	 */
	void Shutdown();

	/**
	 * Called at the beginning of a new frame. The depth values are erased from the depth buffer
	 * and the back buffer is colored according to the color specified by the parameters.
	 * @param red red componenent of the back buffer color (0,1)
	 * @param green green componenent of the back buffer color (0,1)
	 * @param blue blue componenent of the back buffer color (0,1)
	 * @param alpha alpha componenent (opacity) of the back buffer color (0,1)
	 */
	void BeginScene(
		const float red, const float green,
		const float blue, const float alpha
	);
	/**
	 * Tells the swap chain to present the rendered 3D scene. This method should be called at the
	 * end of the frame, after all objects have been drawn.
	 */
	void EndScene() const;

	// TODO: is this method necessary?
	void RefreshMatrix(
		const float screenNear, const float screenDepth,
		const int screenWidth, const int screenHeight
	);

	/**
	 * Returns \a m_device
	 * @return Direct3D device
	 */
	ID3D11Device* GetDevice() const;
	/**
	 * Returns \a m_deviceContext
	 * @return Direct3D device context
	 */
	ID3D11DeviceContext* GetDeviceContext() const;
	/**
	* Returns \a m_swapChain
	* @return Direct3D swap chain
	*/
	IDXGISwapChain* GetSwapChain() const;

	/**
	 * Copies the projection matrix to \p projectionMatrix.
	 * @param[out] projectionMatrix
	 */
	//void GetProjectionMatrix(DirectX::XMMATRIX &projectionMatrix) const;
	///**
	// * Copies the world matrix to \p worldMatrix.
	// * @param[out] worldMatrix
	// */
	//void GetWorldMatrix(DirectX::XMMATRIX &worldMatrix) const;
	///**
	// * Copies the orthographic matrix to \p orthoMatrix.
	// * @param[out] orthoMatrix
	// */
	//void GetOrthoMatrix(DirectX::XMMATRIX &orthoMatrix) const;

	DirectX::XMMATRIX GetProjectionMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetOrthoMatrix() const;


	/**
	 * Copies the video card description into \p cardName and the video card memory to \memory.
     * @param[out] cardName
	 * @param[out] memory
	 */
	void GetVideoCardInfo(char *cardName, int &memory) const;
	ID3D11DepthStencilView* GetDepthStencilView() const;

	/**
	 * This function sets the back buffer as the current render target, which may be necessary
	 * after using render-to-texture or similar methods.
	 */
	void SetBackBufferRenderTarget();

	/* Utility functions for settings */
	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOnAlphaBlendingCoverage();
	void TurnOffAlphaBlending();
	void TurnOnCulling();
	void TurnOffCulling();
	void TurnOnWireframe();
	void TurnOffWireframe();

	void ResetViewport();

private:
	bool m_vsyncEnabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	D3D11_VIEWPORT m_viewport;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterStateWireframe;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterStateNoCulling;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaToCoverageBlendingState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaEnableBlendingState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaDisableBlendingState;

	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
};
};
};

#endif // !_DIRECT3D_H_