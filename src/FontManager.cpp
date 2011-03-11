#if defined(_MSC_VER)
#include "SDL.h"
#include <SDL_TTF.h>
#else
#include "SDL/SDL.h"
#include <SDL_TTF.h>
#endif

#include "FontManager.h"

#include "templates.h"


FontManager::FontManager(void)
: m_iFontCount(0)
, m_pArrayFonts(NULL)
, m_iArraySize(0)
, m_bInitialised(false)
{
	m_iArraySize = 20;
	m_pArrayFonts = new Font*[ m_iArraySize ];
}

FontManager::~FontManager(void)
{
}

/* Get pointer to a (possibly new) font object. FontManager retains ownership!!!! */
Font* FontManager::GetFont( char* szFontName, int iFontSize )
{
	for ( int i = 0 ; i < m_iFontCount ; i++ )
		if ( (m_pArrayFonts[i]->m_iFontSize == iFontSize)
				&& strcmp(m_pArrayFonts[i]->m_szFontName, szFontName ) == 0 )
			return m_pArrayFonts[i];
	// Otherwise font is a new one:
	if ( (m_iFontCount+1) > m_iArraySize )
		m_pArrayFonts = GrowArray( m_pArrayFonts, m_iArraySize, 10 );
	// Array is now big enough
	printf( "Loading font: %d point %s\n", iFontSize, szFontName );
	if ( m_bInitialised )
		m_pArrayFonts[m_iFontCount++] = new Font( TTF_OpenFont( szFontName, iFontSize ), szFontName, iFontSize );
	else
		m_pArrayFonts[m_iFontCount++] = new Font( NULL, szFontName, iFontSize );
	return m_pArrayFonts[m_iFontCount-1];
}

/** Load all of the fonts which have been selected */
void FontManager::LoadFonts()
{
	m_bInitialised = true; // So that future ones get loaded

	for ( int i = 0 ; i < m_iFontCount ; i++ )
	{
		Font* pFont = m_pArrayFonts[i];
		if ( pFont->m_pFont == NULL )
			pFont->m_pFont = TTF_OpenFont( pFont->m_szFontName, pFont->m_iFontSize );
	}
}


