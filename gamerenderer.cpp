/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>

#include <math.h>
#include <memory>

#include "gamerenderer.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GameRenderer::GameRenderer(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      m_Initialized(false)
{
}

GameRenderer::~GameRenderer()
{
}

void GameRenderer::initialize(int width, int height, Design design)
{
    m_pBoardRenderer = std::make_shared<GameBoardRenderer>(width, height, 50, design);
    m_pCoinRenderer = std::make_shared<GameCoinRenderer>(width, height, 50, design);
    m_pBoardRenderer->init();
    m_Initialized = true;

    qglClearColor(QColor(0, 0, 0));
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef HAVE_GLES
    glShadeModel(GL_FLAT);
#endif
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0x0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    qDebug() << "resize: " << size();
//    resize(size().width() + 10, size().height());
}

void GameRenderer::stop()
{
    m_Initialized = false;
}

std::shared_ptr<GameBoardRenderer> GameRenderer::getGameBoardRenderer() const
{
    return m_pBoardRenderer;
}

std::shared_ptr<GameCoinRenderer> GameRenderer::getGameCoinRenderer() const
{
    return m_pCoinRenderer;
}

void GameRenderer::initializeGL()
{

}

void GameRenderer::paintGL()
{
    qglClearColor(QColor(0, 0, 0));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    if (m_Initialized)
    {
        m_pCoinRenderer->draw();
        m_pBoardRenderer->draw();
    }
}

void GameRenderer::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
#ifndef HAVE_GLES
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
    if (m_Initialized)
    {
        m_pBoardRenderer->resize(width, height);
        m_pCoinRenderer->resize(width, height);
    }
}
void GameRenderer::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event);
}


