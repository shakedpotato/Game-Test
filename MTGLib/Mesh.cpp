#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include "DX11Executor.h"
#include "Texture.h"
#include "Mesh.h"

//void Mesh::Test()
//{
//
//	GraphicRenderingExecuter* pRenderer = GameBasicSystemObject<GraphicRenderingExecuter>::Instance();
//
//	D3D11_MAPPED_SUBRESOURCE mr;
//
//	{
//		// �o�b�t�@�ɏ������蓖��
//		HRESULT hr = pRenderer->GetDeviceContext()->Map(
//			instanceBuffer, 
//			0, 
//			D3D11_MAP_WRITE_DISCARD, 
//			0, 
//			&mr);
//		InstancedInfoType* m = ( InstancedInfoType* ) ( mr.pData );
//
//		for( int i = 0; i < currentInstancedCount; i++ )
//		{
//			m[ i ].pos = MyMath::Vector3(2.0f * i, 0.0f, 2.0f * i);
//		}
//
//		pRenderer->GetDeviceContext()->Unmap(instanceBuffer, 0);
//
//	}
//
//	t++;
//
//	if( ( int ) t % 300 == 0 )
//	{
//		currentInstancedCount++;
//	}
//}

void Mesh::Draw()
{
	// Test();

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	// ���_�o�b�t�@�ݒ�
	dx11->SetVertexBuffers( m_VertexBuffer );

	// �C���f�b�N�X�o�b�t�@�ݒ�
	dx11->SetIndexBuffer( m_IndexBuffer );


	for( unsigned short i = 0; i < m_SubsetNum; i++ )
	{
		// �}�e���A���ݒ�
		// pRenderer->SetMaterial( m_SubsetArray[ i ].Material.Material );

		// �e�N�X�`���ݒ�
		dx11->SetTexture( m_SubsetArray[ i ].Material.Texture );

		// �|���S���`��
		dx11->GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		dx11->DrawIndexed( m_SubsetArray[ i ].IndexNum, m_SubsetArray[ i ].StartIndex, 0 );
		// CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// CRenderer::GetDeviceContext()->DrawIndexedInstanced( m_SubsetArray[ i ].IndexNum, 110, m_SubsetArray[ i ].StartIndex, 0, 0 );
	}
	

}


void Mesh::Load(const TCHAR* FileName)
{

	ModelInfo model;
	LoadObj(FileName, &model);

		// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new DX11_SUBSET[ model.SubsetNum ];
		m_SubsetNum = model.SubsetNum;

		for( unsigned short i = 0; i < model.SubsetNum; i++ )
		{
			m_SubsetArray[ i ].StartIndex = model.SubsetArray[ i ].StartIndex;
			m_SubsetArray[ i ].IndexNum = model.SubsetArray[ i ].IndexNum;

			m_SubsetArray[ i ].Material.data = model.SubsetArray[ i ].Material.data;

			m_SubsetArray[ i ].Material.Texture = new Texture();
			m_SubsetArray[ i ].Material.Texture->Load(model.SubsetArray[ i ].Material.TextureName);

		}
	}

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();


	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		dx11->GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

		vertexCount = model.VertexNum;
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		dx11->GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

		indexCount = model.IndexNum;
	}







	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}


void Mesh::Unload()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	instanceBuffer->Release();

	for( unsigned short i = 0; i < m_SubsetNum; i++ )
	{
		m_SubsetArray[ i ].Material.Texture->Unload();
		delete  m_SubsetArray[ i ].Material.Texture;
	}

	delete[] m_SubsetArray;

}

void Mesh::LoadObj(const char* FileName, ModelInfo* Model)
{

	XMFLOAT3* positionArray;
	XMFLOAT3* normalArray;
	XMFLOAT2* texcoordArray;

	unsigned short	positionNum = 0;
	unsigned short	normalNum = 0;
	unsigned short	texcoordNum = 0;
	unsigned short	vertexNum = 0;
	unsigned short	indexNum = 0;
	unsigned short	in = 0;
	unsigned short	subsetNum = 0;

	ModelMaterialInfo* materialArray = nullptr;
	unsigned short	materialNum = 0;

	char str[ 256 ];
	char* s;
	char c;


	FILE* file;
	file = fopen(FileName, "rt");
	assert(file);



	//�v�f���J�E���g
	while( true )
	{
		fscanf(file, "%s", str);

		if( feof(file) != 0 )
			break;

		if( strcmp(str, "v") == 0 )
		{
			positionNum++;
		}
		else if( strcmp(str, "vn") == 0 )
		{
			normalNum++;
		}
		else if( strcmp(str, "vt") == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp(str, "usemtl") == 0 )
		{
			subsetNum++;
		}
		else if( strcmp(str, "f") == 0 )
		{
			in = 0;

			do
			{
				fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//�������m��
	positionArray = new XMFLOAT3[ positionNum ];
	normalArray = new XMFLOAT3[ normalNum ];
	texcoordArray = new XMFLOAT2[ texcoordNum ];


	Model->VertexArray = new Vertex3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new RenderingSubset[ subsetNum ];
	Model->SubsetNum = subsetNum;




	//�v�f�Ǎ�
	XMFLOAT3* position = positionArray;
	XMFLOAT3* normal = normalArray;
	XMFLOAT2* texcoord = texcoordArray;

	unsigned short vc = 0;
	unsigned short ic = 0;
	unsigned short sc = 0;


	fseek(file, 0, SEEK_SET);

	while( true )
	{
		fscanf(file, "%s", str);

		if( feof(file) != 0 )
			break;

		if( strcmp(str, "mtllib") == 0 )
		{
			//�}�e���A���t�@�C��
			fscanf(file, "%s", str);

			char path[ 256 ];
			strcpy(path, "data/MODEL/");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if( strcmp(str, "o") == 0 )
		{
			//�I�u�W�F�N�g��
			fscanf(file, "%s", str);
		}
		else if( strcmp(str, "v") == 0 )
		{
			//���_���W
			fscanf(file, "%f", &position->x);
			fscanf(file, "%f", &position->y);
			fscanf(file, "%f", &position->z);
			//position->x *= SCALE_MODEL;
			//position->y *= SCALE_MODEL;
			//position->z *= SCALE_MODEL;
			position++;
		}
		else if( strcmp(str, "vn") == 0 )
		{
			//�@��
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if( strcmp(str, "vt") == 0 )
		{
			//�e�N�X�`�����W
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp(str, "usemtl") == 0 )
		{
			//�}�e���A��
			fscanf(file, "%s", str);

			if( sc != 0 )
				Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

			Model->SubsetArray[ sc ].StartIndex = ic;


			for( unsigned short i = 0; i < materialNum; i++ )
			{
				if( strcmp(str, materialArray[ i ].Name) == 0 )
				{
					Model->SubsetArray[ sc ].Material.data = materialArray[ i ].data;
					strcpy(Model->SubsetArray[ sc ].Material.TextureName, materialArray[ i ].TextureName);
					strcpy(Model->SubsetArray[ sc ].Material.Name, materialArray[ i ].Name);

					break;
				}
			}

			sc++;

		}
		else if( strcmp(str, "f") == 0 )
		{
			//��
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				Model->VertexArray[ vc ].Position = positionArray[ atoi(s) - 1 ];
				if( s[ strlen(s) + 1 ] != '/' )
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(NULL, "/");
					Model->VertexArray[ vc ].TexCoord = texcoordArray[ atoi(s) - 1 ];
				}
				s = strtok(NULL, "/");
				Model->VertexArray[ vc ].Normal = normalArray[ atoi(s) - 1 ];

				Model->VertexArray[ vc ].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				Model->IndexArray[ ic ] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
			{
				Model->IndexArray[ ic ] = vc - 4;
				ic++;
				Model->IndexArray[ ic ] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;


	fclose(file);


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

void Mesh::LoadMaterial(const char* FileName, ModelMaterialInfo** MaterialArray, unsigned short* MaterialNum)
{
	char str[ 256 ];

	FILE* file;
	file = fopen(FileName, "rt");
	if( file == NULL )
	{
		assert(false);
		return;
	}

	ModelMaterialInfo* materialArray;
	unsigned short materialNum = 0;

	//�v�f���J�E���g
	while( true )
	{
		fscanf(file, "%s", str);

		if( feof(file) != 0 )
			break;


		if( strcmp(str, "newmtl") == 0 )
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new ModelMaterialInfo[ materialNum ];


	//�v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while( true )
	{
		fscanf(file, "%s", str);

		if( feof(file) != 0 )
			break;


		if( strcmp(str, "newmtl") == 0 )
		{
			//�}�e���A����
			mc++;
			fscanf(file, "%s", materialArray[ mc ].Name);
			strcpy(materialArray[ mc ].TextureName, "");
		}
		else if( strcmp(str, "Ka") == 0 )
		{
			//�A���r�G���g
			fscanf(file, "%f", &materialArray[ mc ].data.Ambient.r);
			fscanf(file, "%f", &materialArray[ mc ].data.Ambient.g);
			fscanf(file, "%f", &materialArray[ mc ].data.Ambient.b);
			materialArray[ mc ].data.Ambient.a = 1.0f;
		}
		else if( strcmp(str, "Kd") == 0 )
		{
			//�f�B�t���[�Y
			fscanf(file, "%f", &materialArray[ mc ].data.Diffuse.r);
			fscanf(file, "%f", &materialArray[ mc ].data.Diffuse.g);
			fscanf(file, "%f", &materialArray[ mc ].data.Diffuse.b);
			materialArray[ mc ].data.Diffuse.a = 1.0f;
		}
		else if( strcmp(str, "Ks") == 0 )
		{
			//�X�y�L����
			fscanf(file, "%f", &materialArray[ mc ].data.Specular.r);
			fscanf(file, "%f", &materialArray[ mc ].data.Specular.g);
			fscanf(file, "%f", &materialArray[ mc ].data.Specular.b);
			materialArray[ mc ].data.Specular.a = 1.0f;
		}
		else if( strcmp(str, "Ns") == 0 )
		{
			//�X�y�L�������x
			fscanf(file, "%f", &materialArray[ mc ].data.Shininess);
		}
		else if( strcmp(str, "d") == 0 )
		{
			//�A���t�@
			fscanf(file, "%f", &materialArray[ mc ].data.Diffuse.a);
		}
		else if( strcmp(str, "map_Kd") == 0 )
		{
			//�e�N�X�`��
			fscanf(file, "%s", str);

			char path[ 256 ];
			strcpy(path, "data/MODEL/");
			strcat(path, str);

			strcat(materialArray[ mc ].TextureName, path);
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}
