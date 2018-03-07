 

 
 
void MyGLWidget::calculs_noretall() {
	//L'alçada del model max
	alcada_max = (int)((maxz-minz)/2);
	//Prenem  i  perque son els maxims i els minim del terra (i de l'escena per tant)
	maxim_escena = glm::vec3(5, alcada_max, 5); 
	minim_escena = glm::vec3(-5, 0, -5); //alçada min = -, que es on es situa el terra

	radiEsc = glm::distance(maxim_escena,minim_escena) / 2;
	distancia = radiEsc*2.f;

	//VRP = (maxim_escena+minim_escena); VRP /= 2;
	VRP[0] = (maxim_escena.x+minim_escena.x)/2.0;
	VRP[1] = (maxim_escena.y+minim_escena.y)/2.0;
	VRP[2] = (maxim_escena.z+minim_escena.z)/2.0;
}
 
 
void MyGLWidget::ini_focus() {
	glUniform1i (focuscamaraLoc, focuscamara);
	glUniform3fv(posfocusLoc,1,&posFocus[0]);
	glUniform3fv(colorfocusLoc,1,&colFocus[0]);
}


void MyGLWidget::initialize_Camera ()
{
    viewTransform();
    FOV_inicial = FOV = 2.0*asin(radiEsc/distancia);//(float)M_PI/2.f;
    ra = 1.f;
    
    r = t = radiEsc;
    l = b = -radiEsc;
    
    projectTransform();
}


void MyGLWidget::resizeGL (int w, int h) 
{
	ra = (float)w/(float)h; // ra(v) = raw

	//Recalculem el FOV per tal que no es retalli quan raw<1
	if (w < h) {
        FOV = 2 * atan(tan(FOV_inicial/2.f)/ra);
        b = -radiEsc / ra;
        t = radiEsc / ra;
    }
    if (w > h) {
        l = -radiEsc * ra;
        r = radiEsc * ra;
    }
    
	glViewport(0, 0, w, h);
	projectTransform();
}


void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  
  TG = glm::translate(TG, glm::vec3(-4,0,4));
  
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(-1,0,0));
  
  TG = glm::translate(TG, -centreBase2);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  
  if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ZOOM;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;
    viewTransform ();
  }

  if (DoingInteractive == ZOOM)
  {
    // Fem canvi de FOV
   /* float angle = FOV + (e->y() - yClick)*0.01;
    if( angle > 0 and angle < 3 )
		FOV = angle;*/
	float angle = (e->y() - yClick);
	
	float newfov = FOV + angle*(M_PI/180);
	if( newfov >= 0.10472 and newfov <= 3.14159 )
		FOV = newfov;
		
	emit zoom_signal(FOV*(180/M_PI));
	
    projectTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}
