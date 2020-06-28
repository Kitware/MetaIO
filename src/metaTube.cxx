/*============================================================================
  MetaIO
  Copyright 2000-2010 Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifdef _MSC_VER
#pragma warning(disable:4702)
#pragma warning(disable:4284)
#endif

#include "metaTube.h"

#include <cctype>
#include <cstdio>
#include <string>


#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif

TubePnt::
TubePnt(int dim)
{
  m_NDims = dim;

  m_ID = -1;

  m_X = new float[m_NDims];
  unsigned int i=0;
  for(i=0;i<m_NDims;i++)
    {
    m_X[i] = 0;
    }

  for(i=0;i<4;i++)
    {
    m_Color[i] = 1;
    }

  m_Mark = false;

  m_T = new float[m_NDims];
  m_V1 = new float[m_NDims];
  m_V2 = new float[m_NDims];
  for(unsigned int i=0;i<m_NDims;i++)
    {
    m_T[i] = 0;
    m_V1[i] = 0;
    m_V2[i] = 0;
    }
  m_Alpha1 = 0;
  m_Alpha2 = 0;
  m_Alpha3 = 0;

  m_R = 0;
  m_Medialness = 0;
  m_Ridgeness = 0;
  m_Branchness = 0;
  m_Curvature = 0;
  m_Levelness = 0;
  m_Roundness = 0;
  m_Intensity = 0;

  m_ExtraFields.clear();
}

TubePnt::
TubePnt(const TubePnt * _tubePnt)
{
  CopyInfo( _tubePnt );
}

TubePnt::
~TubePnt()
{
  delete [] m_X;
  delete [] m_T;
  delete [] m_V1;
  delete [] m_V2;

  m_ExtraFields.clear();
}

void TubePnt::
CopyInfo( const TubePnt * _tubePnt )
{
  delete [] m_X;
  delete [] m_T;
  delete [] m_V1;
  delete [] m_V2;

  m_ExtraFields.clear();

  m_NDims = _tubePnt->m_NDims;

  m_X = new float[m_NDims];
  m_T = new float[m_NDims];
  m_V1 = new float[m_NDims];
  m_V2 = new float[m_NDims];
  for( unsigned int i=0; i<m_NDims; ++i )
    {
    m_X[i] = _tubePnt->m_X[i];
    m_T[i] = _tubePnt->m_T[i];
    m_V1[i] = _tubePnt->m_V1[i];
    m_V2[i] = _tubePnt->m_V2[i];
    }
  m_Alpha1 = _tubePnt->m_Alpha1;
  m_Alpha2 = _tubePnt->m_Alpha2;
  m_Alpha3 = _tubePnt->m_Alpha3;
  m_R = _tubePnt->m_R;
  m_Medialness = _tubePnt->m_Medialness;
  m_Ridgeness = _tubePnt->m_Ridgeness;
  m_Branchness = _tubePnt->m_Branchness;
  m_Curvature = _tubePnt->m_Curvature;
  m_Levelness = _tubePnt->m_Levelness;
  m_Roundness = _tubePnt->m_Roundness;
  m_Intensity = _tubePnt->m_Intensity;

  for( unsigned int i=0; i<4; ++i )
    {
    m_Color[i] = _metaPoint->m_Color[i];
    }
  m_Mark = _metaPoint->m_Mark;

  FieldListType::const_iterator it = _metaPoint->m_ExtraFields.begin();
  FieldListType::const_iterator itEnd = _metaPoint->m_ExtraFields.end();
  while(it != itEnd)
    {
    m_ExtraFields.push_back( *it );
    ++it;
    }
}
const TubePnt::FieldListType & TubePnt::
GetExtraFields() const
{
  return m_ExtraFields;
}

int TubePnt::
GetNumberOfExtraFields() const
{
  return m_ExtraFields.size();
}

void TubePnt::
SetNumberOfExtraFields( int size )
{
  m_ExtraFields.resize( size );
}

void TubePnt::
SetField( int indx, const char * name, float value )
{
  FieldType field(name,value);
  m_ExtraFields[ indx ] = field;
}

void TubePnt::
SetField( const char * name, float value )
{
  int indx = this->GetFieldIndex( name );
  if( indx >= 0 )
    {
    m_ExtraFields[indx].second = value;
    }
  else
    {
    this->AddField( name, value );
    }
}

void TubePnt::
AddField(const char* name, float value)
{
  int indx = this->GetFieldIndex( name );
  if( indx != -1 )
    {
    m_ExtraFields[indx].second = value;
    }
  else
    {
    FieldType field(name,value);
    m_ExtraFields.push_back(field);
    }
}

int TubePnt::
GetFieldIndex(const char* name) const
{
  unsigned int count = 0;
  FieldListType::const_iterator it = m_ExtraFields.begin();
  FieldListType::const_iterator itEnd = m_ExtraFields.end();
  while(it != itEnd)
    {
    if(!strcmp((*it).first.c_str(), name))
      {
      return count;
      }
    ++it;
    ++count;
    }
  return -1;
}

float TubePnt::
GetField(int indx) const
{
  if( indx >= 0 && indx < m_ExtraFields.size() )
    {
    return m_ExtraFields[indx].second;
    }
  return -1;
}

float TubePnt::
GetField(const char* name) const
{
  FieldListType::const_iterator it = m_ExtraFields.begin();
  FieldListType::const_iterator itEnd = m_ExtraFields.end();
  while(it != itEnd)
    {
    if(!strcmp((*it).first.c_str(),name))
      {
      return (*it).second;
      }
    ++it;
    }
  return -1;
}

/** MetaTube Constructors */
MetaTube::
MetaTube()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube()" << std::endl;
    }
  Clear();
}


MetaTube::
MetaTube(const char *_headerName)
:MetaObject()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube()" << std::endl;
    }
  Clear();
  Read(_headerName);
}


MetaTube::
MetaTube(const MetaTube *_Tube)
:MetaObject()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube()" << std::endl;
    }
  Clear();
  CopyInfo(_Tube);
}


MetaTube::
MetaTube(unsigned int dim)
:MetaObject(dim)
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube()" << std::endl;
    }
  Clear();
}

/** Destructor */
MetaTube::
~MetaTube()
{
  // Delete the list of pointers to PointObject.
  PointListType::iterator it = m_PointList.begin();
  while(it != m_PointList.end())
  {
    TubePnt * pnt = *it;
    ++it;
    delete pnt;
  }
  m_PointList.clear();
  M_Destroy();
}

//
void MetaTube::
PrintInfo() const
{
  MetaObject::PrintInfo();

  std::cout << "PointDim = " << m_PointDim << std::endl;

  std::cout << "NPoints = " << m_NPoints << std::endl;

  char str[255];
  MET_TypeToString(m_ElementType, str);
  std::cout << "ElementType = " << str << std::endl;

  std::cout << "ParentPoint = " << m_ParentPoint << std::endl;

  if(m_Root)
    {
    std::cout << "Root = " << "True" << std::endl;
    }
  else
    {
    std::cout << "Root = " << "False" << std::endl;
    }

  std::cout << "Artery = " << m_Artery << std::endl;
}

void MetaTube::
CopyInfo(const MetaTube * _object)
{
  Clear();

  MetaObject::CopyInfo(_object);

  PointListType::const_iterator it = _object->GetPoints().begin();
  while(it != _object->GetPoints().end())
    {
    TubePnt * pnt = new TubePnt( *it );
    m_PointList.push_back(pnt);
    ++it;
    }

  m_ParentPoint = _object->ParentPoint();
  m_Artery = _object->Artery();
  m_Root = _object->Root();
}

/** Clear Tube information */
void MetaTube::
Clear()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube: Clear" << std::endl;
    }

  MetaObject::Clear();

  strcpy(m_ObjectTypeName, "Tube");
  strcpy(m_ObjectSubTypeName, "");

  m_ElementType = MET_FLOAT;

  m_ParentPoint = -1;
  m_Root = false;
  m_Artery = true;

  // Delete the list of pointers to PointObjects.
  PointListType::iterator it = m_PointList.begin();
  while(it != m_PointList.end())
  {
    TubePnt * pnt = *it;
    ++it;
    delete pnt;
  }
  m_PointList.clear();

  m_NPoints = 0;
  if( m_NDims == 2 )
    {
    m_PointDim = "id x y red green blue alpha mark r rn mn bn cv lv ro in tx ty v1x v1y a1 a2";
    }
  else
    {
    m_PointDim = "id x y z red green blue alpha mark r rn mn bn cv lv ro in tx ty tz v1x v1y v1z v2x v2y v2z a1 a2 a3";
    }

}

const char* MetaPointObject::
PointDim() const
{
  return m_PointDim.c_str();
}

void MetaPointObject::
NPoints(int npnt)
{
  m_NPoints = npnt;
}

int MetaPointObject::
NPoints() const
{
  return m_NPoints;
}


/** Destroy Tube information */
void MetaTube::
M_Destroy()
{
  MetaObject::M_Destroy();
}

/** Set Read fields */
void MetaTube::
M_SetupReadFields()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube: M_SetupReadFields"
                        << std::endl;
    }

  MetaObject::M_SetupReadFields();

  MET_FieldRecordType * mF;

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ParentPoint", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Root", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Artery", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "PointDim", MET_STRING, true);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "NPoints", MET_INT, true);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Points", MET_NONE, true);
  mF->terminateRead = true;
  m_Fields.push_back(mF);
}

void MetaTube::
M_SetupWriteFields()
{
  MetaObject::M_SetupWriteFields();

  MET_FieldRecordType * mF;

  if(m_ParentPoint>=0 && m_ParentID>=0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ParentPoint", MET_INT, m_ParentPoint);
    m_Fields.push_back(mF);
    }

  if(m_Root)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Root", MET_STRING, strlen("True"), "True");
    m_Fields.push_back(mF);
    }
  else
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Root", MET_STRING, strlen("False"), "False");
    m_Fields.push_back(mF);
    }

  if(m_Artery)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Artery", MET_STRING, strlen("True"), "True");
    m_Fields.push_back(mF);
    }
  else
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Artery", MET_STRING, strlen("False"), "False");
    m_Fields.push_back(mF);
    }

  if( m_NDims == 2 )
    {
    m_PointDim = "id x y red green blue alpha mark r rn mn bn cv lv ro in tx ty v1x v1y a1 a2";
    }
  else
    {
    m_PointDim = "id x y z red green blue alpha mark r rn mn bn cv lv ro in tx ty tz v1x v1y v1z v2x v2y v2z a1 a2 a3";
    }

  // All the points in the tube have the same number of fields
  const MetaPoint::FieldListType & extraList =
    (*(m_PointList.begin()))->GetExtraFields();
  MetaPoint::FieldListType::const_iterator itFields = extraList.begin();
  MetaPoint::FieldListType::const_iterator itFieldsEnd = extraList.end();
  while(itFields !=  itFieldsEnd)
    {
    m_PointDim += " ";
    m_PointDim += (*itFields).first;
    ++itFields;
    }

  mF = MET_GetFieldRecord("PointDim", &m_Fields);
  MET_InitWriteField(mF, "PointDim", MET_STRING, m_PointDim.size(),
    m_PointDim.c_str() );
  
  m_NPoints = (int)m_PointList.size();
  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "NPoints", MET_INT, m_NPoints);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "Points", MET_NONE);
  m_Fields.push_back(mF);
}

/** Return the position given the name of the field */
int MetaTube::
M_GetPosition(const char* name, std::vector<bool> & used) const
{
  std::vector<PositionType>::const_iterator it = m_Positions.begin();
  std::vector<bool>::iterator itUsed = used.begin();
  std::vector<PositionType>::const_iterator itEnd = m_Positions.end();
  while(it != itEnd)
    {
    if(!strcmp((*it).first.c_str(),name))
      {
      *itUsed = true;
      return (*it).second;
      }
    ++it;
    ++itUsed;
    }

  return -1;
}

float MetaTube::
M_GetFloatFromBinaryData( int pos, char * _data, int readSize ) const
{
  if( pos >= 0 && pos < readSize )
    {
    float tf;
    char * const num = (char *)(&tf);
    for(unsigned int k=0; k<sizeof(float) && pos+k<readSize; k++)
      {
      num[k] = _data[pos+k];
      }
    MET_SwapByteIfSystemMSB(&tf, MET_FLOAT);
    return (float)tf;
    }
  return -1;
}

bool MetaTube::
M_Read()
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube: M_Read: Loading Header"
                        << std::endl;
    }

  if(!MetaObject::M_Read())
    {
    std::cout << "MetaTube: M_Read: Error parsing file"
                        << std::endl;
    return false;
    }

  if(META_DEBUG)
    {
    std::cout << "MetaTube: M_Read: Parsing Header"
                        << std::endl;
    }

  MET_FieldRecordType * mF;

  mF = MET_GetFieldRecord("ParentPoint", &m_Fields);
  if(mF->defined)
    {
    m_ParentPoint= (int)mF->value[0];
    }

  m_Root = false;
  mF = MET_GetFieldRecord("Root", &m_Fields);
  if(mF->defined)
    {
    if(*((char *)(mF->value)) == 'T'
       || *((char*)(mF->value)) == 't'
       || *((char*)(mF->value)) == '1')
      {
      m_Root = true;
      }
    else
      {
      m_Root = false;
      }
    }

  m_Artery = true;
  mF = MET_GetFieldRecord("Artery", &m_Fields);
  if(mF->defined)
    {
    if(*((char *)(mF->value)) == 'T' || *((char*)(mF->value)) == 't')
      {
      m_Artery = true;
      }
    else
      {
      m_Artery = false;
      }
    }

  mF = MET_GetFieldRecord("NPoints", &m_Fields);
  if(mF->defined)
    {
    m_NPoints = (int)mF->value[0];
    }

  mF = MET_GetFieldRecord("ElementType", &m_Fields);
  if(mF->defined)
    {
    MET_StringToType((char *)(mF->value), &m_ElementType);
    }

  mF = MET_GetFieldRecord("PointDim", &m_Fields);
  if(mF->defined)
    {
    strcpy(m_PointDim, (char *)(mF->value));
    }

  int pntDim;
  char** pntVal = nullptr;
  char pointDim[4096];

  for(unsigned t = 0; t<this->m_PointDim.size(); t++)
    {
    pointDim[t] = this->m_PointDim[t];
    }
  pointDim[m_PointDim.size()] = '\0';

  MET_StringToWordArray(pointDim, &pntDim, &pntVal);

  if(META_DEBUG)
    {
    std::cout << "MetaPointObject: Parsing point dim" << std::endl;
    }

  m_Positions.clear();
  std::vector<bool> positionUsed;
  for(unsigned int i=0; i<pntDim; i++)
    {
    PositionType p(pntVal[i], i);
    m_Positions.push_back(p);
    positionUsed.push_back(false);
    }

  for(unsigned int i=0; i<pntDim; i++)
    {
    delete [] pntVal[i];
    }
  delete [] pntVal;

  float v[100];

  if(m_Event)
    {
    m_Event->StartReading(m_NPoints);
    }

  int posId = M_GetPosition("id", positionUsed);
  int posX = M_GetPosition("x", positionUsed);
  int posY = M_GetPosition("y", positionUsed);
  int posZ = M_GetPosition("z", positionUsed);
  int posRed = M_GetPosition("red", positionUsed);
  int posGreen = M_GetPosition("green", positionUsed);
  int posBlue = M_GetPosition("blue", positionUsed);
  int posAlpha = M_GetPosition("alpha", positionUsed);
  int posMark = M_GetPosition("mark", positionUsed);
  if( posMark == -1 )
    { posMark = M_GetPosition("mk", positionUsed); }
  int posR = M_GetPosition("r", positionUsed);
  if( posR == -1 )
    { posR = M_GetPosition("R", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("radius", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("Radius", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("rad", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("Rad", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("s", positionUsed); }
  else if( posR == -1 )
    { posR = M_GetPosition("S", positionUsed); }
  int posRn = M_GetPosition("rn", positionUsed);
  int posMn = M_GetPosition("mn", positionUsed);
  int posBn = M_GetPosition("bn", positionUsed);
  int posCv = M_GetPosition("cv", positionUsed);
  int posLv = M_GetPosition("lv", positionUsed);
  int posRo = M_GetPosition("ro", positionUsed);
  int posIn = M_GetPosition("in", positionUsed);
  int posTx = M_GetPosition("tx", positionUsed);
  int posTy = M_GetPosition("ty", positionUsed);
  int posTz = M_GetPosition("tz", positionUsed);
  int posV1x = M_GetPosition("v1x", positionUsed);
  int posV1y = M_GetPosition("v1y", positionUsed);
  int posV1z = M_GetPosition("v1z", positionUsed);
  int posV2x = M_GetPosition("v2x", positionUsed);
  int posV2y = M_GetPosition("v2y", positionUsed);
  int posV2z = M_GetPosition("v2z", positionUsed);
  int posA1 = M_GetPosition("a1", positionUsed);
  int posA2 = M_GetPosition("a2", positionUsed);
  int posA3 = M_GetPosition("a3", positionUsed);

  if(m_BinaryData)
    {
    int elementSize;
    MET_SizeOfType(m_ElementType, &elementSize);
    int readSize = m_NPoints*pntDim*elementSize;

    char* _data = new char[readSize];
    m_ReadStream->read((char *)_data, readSize);

    int gc = static_cast<int>(m_ReadStream->gcount());
    if(gc != readSize)
      {
      std::cout << "MetaLine: m_Read: data not read completely"
                << std::endl;
      std::cout << "   ideal = " << readSize
                << " : actual = " << gc << std::endl;
      delete [] _data;
      return false;
      }

    for(unsigned int j=0; j<m_NPoints; j++)
      {
      MetaPoint* pnt = new MetaPoint(m_NDims);

      if( posId >= 0 )
        {
        pnt->m_ID = M_GetFloatFromBinaryData( posId, _data, readSize );
        }
      if( posX >= 0 )
        {
        pnt->m_X[0] = M_GetFloatFromBinaryData( posX, _data, readSize );
        }
      if( posY >= 0 )
        {
        pnt->m_X[1] = M_GetFloatFromBinaryData( posY, _data, readSize );
        }
      if( m_NDims == 3 && posZ >= 0 )
        {
        pnt->m_X[2] = M_GetFloatFromBinaryData( posZ, _data, readSize );
        }
      if( posRed >= 0 )
        {
        pnt->m_Color[0] = M_GetFloatFromBinaryData( posRed, _data, readSize );
        }
      if( posGreen >= 0 )
        {
        pnt->m_Color[1] = M_GetFloatFromBinaryData( posGreen, _data, readSize );
        }
      if( posBlue >= 0 )
        {
        pnt->m_Color[2] = M_GetFloatFromBinaryData( posBlue, _data, readSize );
        }
      if( posMark >= 0 )
        {
        if( M_GetFloatFromBinaryData( posMark, _data, readSize ) != 0 )
          {
          pnt->m_Mark = true;
          }
        else
          {
          pnt->m_Mark = false;
          }
        }
      if( posR != -1 )
        {
        pnt->m_R = M_GetFloatFromBinaryData( posR, _data, readSize );
        }
      if( posMn != -1 )
        {
        pnt->m_Medialness = M_GetFloatFromBinaryData( posMn, _data, readSize );
        }
      if( posRn != -1 )
        {
        pnt->m_Ridgeness = M_GetFloatFromBinaryData( posRn, _data, readSize );
        }
      if( posBn != -1 )
        {
        pnt->m_Branchness = M_GetFloatFromBinaryData( posBn, _data, readSize );
        }
      if( posCv != -1 )
        {
        pnt->m_Curvature = M_GetFloatFromBinaryData( posCv, _data, readSize );
        }
      if( posLv != -1 )
        {
        pnt->m_Levelness = M_GetFloatFromBinaryData( posLv, _data, readSize );
        }
      if( posRo != -1 )
        {
        pnt->m_Roundness = M_GetFloatFromBinaryData( posRo, _data, readSize );
        }
      if( posIn != -1 )
        {
        pnt->m_Intensity = M_GetFloatFromBinaryData( posIn, _data, readSize );
        }
      if( posTx != -1 )
        {
        pnt->m_T[0] = M_GetFloatFromBinaryData( posTx, _data, readSize );
        }
      if( posTy != -1 )
        {
        pnt->m_T[1] = M_GetFloatFromBinaryData( posTy, _data, readSize );
        }
      if( posTz != -1 )
        {
        pnt->m_T[2] = M_GetFloatFromBinaryData( posTz, _data, readSize );
        }
      if( posV1x != -1 )
        {
        pnt->m_V1[0] = M_GetFloatFromBinaryData( posV1x, _data, readSize );
        }
      if( posV1y != -1 )
        {
        pnt->m_V1[1] = M_GetFloatFromBinaryData( posV1y, _data, readSize );
        }
      if( posV1z != -1 )
        {
        pnt->m_V1[2] = M_GetFloatFromBinaryData( posV1z, _data, readSize );
        }
      if( posV2x != -1 )
        {
        pnt->m_V2[0] = M_GetFloatFromBinaryData( posV2x, _data, readSize );
        }
      if( posV2y != -1 )
        {
        pnt->m_V2[1] = M_GetFloatFromBinaryData( posV2y, _data, readSize );
        }
      if( posV2z != -1 )
        {
        pnt->m_V2[2] = M_GetFloatFromBinaryData( posV2z, _data, readSize );
        }
      if( posA1 != -1 )
        {
        pnt->m_Alpha1 = M_GetFloatFromBinaryData( posA1, _data, readSize );
        }
      if( posA2 != -1 )
        {
        pnt->m_Alpha2 = M_GetFloatFromBinaryData( posA2, _data, readSize );
        }
      if( posA3 != -1 )
        {
        pnt->m_Alpha3 = M_GetFloatFromBinaryData( posA3, _data, readSize );
        }

      std::vector<PositionType>::const_iterator itFields =
        m_Positions.begin();
      std::vector<PositionType>::const_iterator itUsed =
        positionUsed.begin();
      std::vector<PositionType>::const_iterator itFieldsEnd =
        m_Positions.end();
      while(itFields !=  itFieldsEnd)
        {
        if( !(*itUsed) )
          {
          pos = M_GetPosition((*itFields).first.c_str());
          if( pos >= 0 )
            {
            float tf = M_GetFloatFromBinaryData( pos, _data, readSize );
            pnt->AddField((*itFields).first.c_str(), tf);
            }
          }
        ++itFields;
        ++itUsed;
        }

      m_PointList.push_back(pnt);
      }
    delete [] _data;
    }
  else
    {
    for(j=0; j<m_NPoints; j++)
      {
      if(m_Event)
        {
        m_Event->SetCurrentIteration(j+1);
        }

      for(unsigned int k=0; k<pntDim; k++)
        {
        *m_ReadStream >> v[k];
        m_ReadStream->get();
        }

      MetaPoint * pnt = new MetaPoint(m_NDims);

      if( posX >= 0 )
        {
        pnt->m_X[0] = v[posX];
        }
      if( posId >= 0 )
        {
        pnt->m_ID = v[posId];
        }
      if( posRed >= 0 )
        {
        pnt->m_Color[0] = v[posRed];
        }
      if( posGreen >= 0 )
        {
        pnt->m_Color[1] = v[posGreen];
        }
      if( posBlue >= 0 )
        {
        pnt->m_Color[2] = v[posBlue];
        }
      if( posAlpha >= 0 )
        {
        pnt->m_Color[3] = v[posAlpha];
        }
      if( posMark >= 0 )
        {
        pnt->m_Mark = (v[posMark]!=0)?true:false;
        }
      if( posR >= 0 )
        {
        pnt->m_R = v[posR];
        }
      if( posRn >= 0 )
        {
        pnt->m_Ridgeness = v[posRn];
        }
      if( posMn >= 0 )
        {
        pnt->m_Medialness = v[posMn];
        }
      if( posBn >= 0 )
        {
        pnt->m_Branchness = v[posBn];
        }
      if( posCv >= 0 )
        {
        pnt->m_Curvature = v[posCv];
        }
      if( posLv >= 0 )
        {
        pnt->m_Levelness = v[posLv];
        }
      if( posRo >= 0 )
        {
        pnt->m_Roundness = v[posRo];
        }
      if( posIn >= 0 )
        {
        pnt->m_Intensity = v[posIn];
        }
      if( posTx >= 0 )
        {
        pnt->m_T[0] = v[posTx];
        }
      if( posTy >= 0 )
        {
        pnt->m_T[1] = v[posTy];
        }
      if( posTz >= 0 )
        {
        pnt->m_T[2] = v[posTz];
        }
      if( posV1x >= 0 )
        {
        pnt->m_V1[0] = v[posV1x];
        }
      if( posV1y >= 0 )
        {
        pnt->m_V1[1] = v[posV1y];
        }
      if( posV1z >= 0 )
        {
        pnt->m_V1[2] = v[posV1z];
        }
      if( posV2x >= 0 )
        {
        pnt->m_V2[0] = v[posV2x];
        }
      if( posV2y >= 0 )
        {
        pnt->m_V2[1] = v[posV2y];
        }
      if( posV2z >= 0 )
        {
        pnt->m_V2[2] = v[posV2z];
        }
      if( posA1 >= 0 )
        {
        pnt->m_Alpha1 = v[posA1];
        }
      if( posA2 >= 0 )
        {
        pnt->m_Alpha2 = v[posA2];
        }
      if( posA3 >= 0 )
        {
        pnt->m_Alpha3 = v[posA3];
        }

      std::vector<PositionType>::const_iterator itFields =
        m_Positions.begin();
      std::vector<PositionType>::const_iterator itUsed =
        positionUsed.begin();
      std::vector<PositionType>::const_iterator itFieldsEnd =
        m_Positions.end();
      while(itFields !=  itFieldsEnd)
        {
        if( !(*itUsed) )
          {
          std::string fldstr = (*itFields).first;
          pnt->AddField( fldstr.c_str(),
            v[this->M_GetPosition(fldstr.c_str())] );
          }
        ++itFields;
        ++itUsed;
        }

      m_PointList.push_back(pnt);
      }

    char c = ' ';
    while( (c!='\n') && (!m_ReadStream->eof()))
      {
      c = static_cast<char>(m_ReadStream->get());
      // to avoid unrecognize charactere
      }
    }

  if(m_Event)
    {
    m_Event->StopReading();
    }

  return true;
}

bool MetaTube::
M_Write()
{

  // Convert point variables to ExtraFields for them to be written
  // by base class.

  TubePnt::FieldType newField;
  PointListType::iterator it = m_PointList.begin();
  PointListType::iterator itEnd = m_PointList.end();
  int lastExtraField = (*it)->m_ExtraFields.size();
  while( it != itEnd )
    {
    TubePnt * pnt = static_cast<TubePnt*>(*it);
    if( pnt->m_R != -1 )
      {
      newField.first = "r";
      newField.second = pnt->m_R;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Ridgeness != -1 )
      {
      newField.first = "rn";
      newField.second = pnt->m_Ridgeness;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Medialness != -1 )
      {
      newField.first = "mn";
      newField.second = pnt->m_Medialness;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Branchness != -1 )
      {
      newField.first = "bn";
      newField.second = pnt->m_Branchness;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Curvature != -1 )
      {
      newField.first = "cv";
      newField.second = pnt->m_Curvature;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Levelness != -1 )
      {
      newField.first = "lv";
      newField.second = pnt->m_Levelness;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Roundness != -1 )
      {
      newField.first = "ro";
      newField.second = pnt->m_Roundness;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Intensity != -1 )
      {
      newField.first = "in";
      newField.second = pnt->m_Intensity;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_T[0] != -1 )
      {
      newField.first = "tx";
      newField.second = pnt->m_T[0];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_T[1] != -1 )
      {
      newField.first = "ty";
      newField.second = pnt->m_T[1];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_T[2] != -1 )
      {
      newField.first = "tz";
      newField.second = pnt->m_T[2];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_V1[0] != -1 )
      {
      newField.first = "v1x";
      newField.second = pnt->m_V1[0];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_V1[1] != -1 )
      {
      newField.first = "v1y";
      newField.second = pnt->m_V1[1];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_V1[2] != -1 )
      {
      newField.first = "v1z";
      newField.second = pnt->m_V1[2];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_V2[0] != -1 )
      {
      newField.first = "v2x";
      newField.second = pnt->m_V2[0];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_V2[1] != -1 )
      {
      newField.first = "v2y";
      newField.second = pnt->m_V2[1];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_V2[2] != -1 )
      {
      newField.first = "v2z";
      newField.second = pnt->m_V2[2];
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Alpha1 != -1 )
      {
      newField.first = "a1";
      newField.second = pnt->m_Alpha1;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( pnt->m_Alpha2 != -1 )
      {
      newField.first = "a2";
      newField.second = pnt->m_Alpha2;
      pnt->m_ExtraFields.push_back(newField);
      }
    if( m_NDims == 3 && pnt->m_Alpha3 != -1 )
      {
      newField.first = "a3";
      newField.second = pnt->m_Alpha3;
      pnt->m_ExtraFields.push_back(newField);
      }
    ++it;
    }
  
  bool returnVal = true;
  if(!MetaObject::M_Write())
    {
    std::cout << "MetaTube: M_Read: Error parsing file"
                        << std::endl;
    returnVal = false;
    }

  // Once the local vars have been written as ExtraFields, remove them
  // from being ExtraFields.
  if( lastExtraField == 0 )
    {
    it = m_PointList.begin();
    itEnd = m_PointList.end();
    while( it != itEnd )
      {
      (*it)->m_ExtraFields.clear();
      ++it;
      }
    }
  else
    {
    it = m_PointList.begin();
    itEnd = m_PointList.end();
    while( it != itEnd )
      {
      (*it)->m_ExtraFields.erase(
        (*it)->m_ExtraFields.begin()+lastExtraField,
        (*it)->m_ExtraFields.end() );
      ++it;
      }
    }

  return returnVal;

}

#if (METAIO_USE_NAMESPACE)
};
#endif
