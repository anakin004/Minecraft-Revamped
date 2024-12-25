#include "Block.h"



Block::Block(uchar minX, uchar minY, uchar maxX, uchar maxY)
{
	topMinX = minX;
	topMinY = minY;
	topMaxX = maxX;
	topMaxY = maxY;

	bottomMinX = minX;
	bottomMinY = minY;
	bottomMaxX = maxX;
	bottomMaxY = maxY;

	sideMinX = minX;
	sideMinY = minY;
	sideMaxX = maxX;
	sideMaxY = maxY;
}

Block::Block(uchar minX, uchar minY, uchar maxX, uchar maxY, uchar spriteSize)
{
	topMinX = minX * spriteSize;
	topMinY = minY * spriteSize;
	topMaxX = maxX * spriteSize;
	topMaxY = maxY * spriteSize;

	bottomMinX = minX * spriteSize;
	bottomMinY = minY * spriteSize;
	bottomMaxX = maxX * spriteSize;
	bottomMaxY = maxY * spriteSize;

	sideMinX = minX * spriteSize;
	sideMinY = minY * spriteSize;
	sideMaxX = maxX * spriteSize;
	sideMaxY = maxY * spriteSize;

}

Block::Block(uchar topMinX, uchar topMinY, uchar topMaxX, uchar topMaxY,
	uchar bottomMinX, uchar bottomMinY, uchar bottomMaxX, uchar bottomMaxY,
	uchar sideMinX, uchar sideMinY, uchar sideMaxX, uchar sideMaxY)
{
	this->topMinX = topMinX;
	this->topMinY = topMinY;
	this->topMaxX = topMaxX;
	this->topMaxY = topMaxY;

	this->bottomMinX = bottomMinX;
	this->bottomMinY = bottomMinY;
	this->bottomMaxX = bottomMaxX;
	this->bottomMaxY = bottomMaxY;

	this->sideMinX = sideMinX;
	this->sideMinY = sideMinY;
	this->sideMaxX = sideMaxX;
	this->sideMaxY = sideMaxY;
}

Block::Block(uchar topMinX, uchar topMinY, uchar topMaxX, uchar topMaxY, uchar bottomMinX, uchar bottomMinY, uchar bottomMaxX, uchar bottomMaxY, uchar sideMinX, uchar sideMinY, uchar sideMaxX, uchar sideMaxY, uchar spriteSize)
{
	this->topMinX = topMinX * spriteSize;
	this->topMinY = topMinY * spriteSize;
	this->topMaxX = topMaxX * spriteSize;
	this->topMaxY = topMaxY * spriteSize;

	this->bottomMinX = bottomMinX * spriteSize;
	this->bottomMinY = bottomMinY * spriteSize;
	this->bottomMaxX = bottomMaxX * spriteSize;
	this->bottomMaxY = bottomMaxY * spriteSize;

	this->sideMinX = sideMinX * spriteSize;
	this->sideMinY = sideMinY * spriteSize;
	this->sideMaxX = sideMaxX * spriteSize;
	this->sideMaxY = sideMaxY * spriteSize;
}
